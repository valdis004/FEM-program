#include "solver.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femelement.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include <Eigen/SparseLU>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/SparseCore/SparseVector.h>
#include <QVector>
#include <cstddef>
#include <qdebug.h>
#include <qglobal.h>
#include <stdexcept>

void Solver::setParams(size_t i, const AbstractFemElement *element,
                       unsigned &correction, unsigned &curDof,
                       unsigned &localId, unsigned &nodeId, unsigned &fullDof) {
  auto data = element->data;
  correction = 0;
  localId = data.LOCAL_ID_FROM_STIFFMAT[i];
  nodeId = element->nodes[localId]->id;
  curDof = data.FULL_DOF_COUNT;
  fullDof = curDof;

  if (!data.IS_FULL_DOF) {
    unsigned corCount = nodeId / data.BAD_DOF_BEGIN;
    unsigned corFromCurElem = data.BAD_DOF_MAP[i];
    correction = corCount * data.BAD_DOF_BEGIN + corFromCurElem;
    curDof = data.DOF_MAP[localId];
  }
}

unsigned Solver::getGlobalIndex(size_t i, const AbstractFemElement *element) {
  unsigned correction;
  unsigned localId;
  unsigned nodeId;
  unsigned curDof;
  unsigned fullDof;
  setParams(i, element, correction, curDof, localId, nodeId, fullDof);

  return nodeId * fullDof + i % curDof - correction;
}

unsigned
Solver::getGlobalIndexAndSetLoad(size_t i, const AbstractFemElement *element,
                                 SparseVector<double> &globalLoadVector) {
  unsigned correction;
  unsigned localId;
  unsigned nodeId;
  unsigned curDof;
  unsigned fullDof;
  setParams(i, element, correction, curDof, localId, nodeId, fullDof);

  unsigned dofIndex = i % curDof;
  double value = element->nodes[localId]->nodeLoad->values[dofIndex];
  double globalId = nodeId * fullDof + dofIndex - correction;
  globalLoadVector.coeffRef(globalId) = value;

  return globalId;
}

std::pair<SparseMatrix<double>, SparseVector<double>>
Solver::getGlobalStiffMatrixAndLoadVector(Mesh *mesh) {
  globalMatrixSize = mesh->globaStiffMatrixSize;
  SparseMatrix<double> globalStiffMatrix(globalMatrixSize, globalMatrixSize);
  SparseVector<double> globalLoadVector(globalMatrixSize);
  unsigned count = 0;

  auto elements = mesh->elements;
  for (AbstractFemElement *element : elements) {
    emit newElementStiffMatrixStep(count++);

    ElementData data = element->data;
    QVector<Node *> nodes = element->nodes;
    MatrixXd localStiffMatrix = element->getLocalStiffMatrix();

    for (size_t i = 0; i < data.STIFF_MATRIX_SIZE; i++) {
      unsigned colGlobId =
          getGlobalIndexAndSetLoad(i, element, globalLoadVector);
      for (size_t j = 0; j < data.STIFF_MATRIX_SIZE; j++) {
        unsigned rowGlobId = getGlobalIndex(j, element);
        double loc = localStiffMatrix(i, j);
        globalStiffMatrix.coeffRef(rowGlobId, colGlobId) +=
            localStiffMatrix(i, j);
      }
    }
  }
  return {globalStiffMatrix, globalLoadVector};
}

void Solver::applyBaundaryConditions(SparseMatrix<double> &globalMatrix,
                                     SparseVector<double> &globalVector,
                                     Mesh *mesh) {

  auto nodes = mesh->nodes;

  for (const auto &node : nodes) {
    if (!node->nodeDisplacement)
      continue;

    short countToZero = node->nodeDisplacement->nodesCountToZero;
    for (size_t i = 0; i < countToZero; i++) {
      unsigned id = node->nodeDisplacement->nodeIdsToZero[i];
      globalVector.coeffRef(id) = 0;
      for (size_t j = 0; j < globalMatrixSize; j++) {
        if (j == id)
          globalMatrix.coeffRef(id, j) = 1;
        else {
          globalMatrix.coeffRef(id, j) = 0;
          globalMatrix.coeffRef(j, id) = 0;
        }
      }
    }
  }
}

void Solver::calculate(Mesh *mesh) {
  auto stiffAndLoad = getGlobalStiffMatrixAndLoadVector(mesh);
  auto stiff = stiffAndLoad.first;
  auto load = stiffAndLoad.second;

  // for (size_t i = 0; i < stiff.rows(); i++) {
  //   for (size_t j = 0; j < stiff.cols(); j++) {
  //     qDebug() << stiff.coeff(i, j);
  //   }
  //   qDebug() << "\n\n\n";
  // }

  emit applyBaundaryConditionsStep();
  applyBaundaryConditions(stiff, load, mesh);

  // qDebug() << " LOAD VECTOR  ";

  // for (size_t i = 0; i < load.size(); i++) {
  //   qDebug() << i << " " << load.coeff(i) << " ";
  // }

  emit solveSystemStep();
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
  solver.compute(stiff); // Шаг 1: факторизация/анализ
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error("Solver info not success");
  }

  SparseVector<double> u = solver.solve(load); // Шаг 2: решение

  // for (size_t i = 0; i < u.size(); i++) {
  //   qDebug() << i << " " << u.coeff(i) << "\n";
  // }

  void getOutputStep();
  setOutputValuesToNodes(mesh, u);
  emit calcFinishedStep();
}

void Solver::setOutputValuesToNodes(Mesh *mesh,
                                    const SparseVector<double> &globalU) {
  bool flag = true;

  double maxAbsValues[ElementProvider::outputCounts]{0};
  double maxValues[ElementProvider::outputCounts]{0};
  double minValues[ElementProvider::outputCounts]{0};
  short count = mesh->elements.first()->data.OUTPUT_VALUES_COUNT;
  this->data = &(mesh->elements.first()->data);

  for (auto &element : mesh->elements) {
    auto data = element->data;

    VectorXd elementU{data.STIFF_MATRIX_SIZE};
    for (size_t i = 0; i < data.STIFF_MATRIX_SIZE; i++) {
      unsigned globalId = getGlobalIndex(i, element);
      elementU(i) = globalU.coeff(globalId);
    }

    for (size_t i = 0; i < data.MAIN_NODES_COUNT; i++) {
      Node *curNode = element->nodes[i];

      QVector<double> outputValues = element->getResultVector(
          elementU, data.MAIN_NODES_XI_SET[i], data.MAIN_NODES_ETA_SET[i]);
      if (flag) {
        for (size_t i = 0; i < outputValues.size(); i++) {
          maxAbsValues[i] = abs(outputValues[i]);
          minValues[i] = outputValues[i];
          maxValues[i] = outputValues[i];
        }
        flag = false;
      }
      curNode->outputValues = outputValues;

      for (size_t j = 0; j < outputValues.size(); j++) {
        if (abs(outputValues[j]) > maxAbsValues[j]) {
          maxAbsValues[j] = abs(outputValues[j]);
        }

        if (outputValues[j] > maxValues[j]) {
          maxValues[j] = outputValues[j];
        }

        if (outputValues[j] < minValues[j]) {
          minValues[j] = outputValues[j];
        }
      }
    }
  }

  for (size_t j = 0; j < count; j++) {
    this->maxAbsValues.push_back(maxAbsValues[j]);
    this->maxValues.push_back(maxValues[j]);
    this->minValues.push_back(minValues[j]);
  }
}

// void Solver::updateProgress(QMessageBox *mes, int count) {
//   mes->setText(
//       QString("Creating local stiff matrix for element %1").arg(count));
// }

// void Solver::showResult(QMessageBox *mes) {
//   mes->setText(QString("Global stiff matrix sucsesfully created"));
// }