#include "solver.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include <Eigen/SparseLU>
#include <Eigen/src/SparseCore/SparseVector.h>
#include <QVector>
#include <cstddef>
#include <qdebug.h>
#include <qglobal.h>
#include <stdexcept>

void Solver::setParams(size_t i, const AbstractElement *element,
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

unsigned Solver::getGlobalIndex(size_t i, const AbstractElement *element) {
  unsigned correction;
  unsigned localId;
  unsigned nodeId;
  unsigned curDof;
  unsigned fullDof;
  setParams(i, element, correction, curDof, localId, nodeId, fullDof);

  return nodeId * fullDof + i % curDof - correction;
}

unsigned
Solver::getGlobalIndexAndSetLoad(size_t i, const AbstractElement *element,
                                 SparseVector<double> globalLoadVector) {
  unsigned correction;
  unsigned localId;
  unsigned nodeId;
  unsigned curDof;
  unsigned fullDof;
  setParams(i, element, correction, curDof, localId, nodeId, fullDof);

  unsigned dofIndex = i % curDof;
  globalLoadVector.insert(i) =
      element->nodes[localId]->nodeLoad->values[dofIndex];

  return nodeId * fullDof + dofIndex - correction;
}

std::pair<SparseMatrix<double>, SparseVector<double>>
Solver::getGlobalStiffMatrixAndLoadVector(Mesh *mesh) {
  globalMatrixSize = mesh->globaStiffMatrixSize;
  SparseMatrix<double> globalStiffMatrix(globalMatrixSize, globalMatrixSize);
  SparseVector<double> globalLoadVector(globalMatrixSize);

  auto elements = mesh->elements;
  for (AbstractElement *element : elements) {
    unsigned count = 0;
    emit progressChanged(count++);

    ElementData data = element->data;
    QVector<Node *> nodes = element->nodes;
    MatrixXd localStiffMatrix = element->getLocalStiffMatrix();

    // QVector<Eigen::Triplet<double>> tripletList;
    // tripletList.reserve(data.STIFF_MATRIX_SIZE * data.STIFF_MATRIX_SIZE);

    for (size_t i = 0; i < data.STIFF_MATRIX_SIZE; i++) {
      unsigned colGlobId =
          getGlobalIndexAndSetLoad(i, element, globalLoadVector);
      for (size_t j = 0; j < data.STIFF_MATRIX_SIZE; j++) {
        unsigned rowGlobId = getGlobalIndex(j, element);
        globalStiffMatrix.coeffRef(rowGlobId, colGlobId) +=
            localStiffMatrix(i, j);

        // tripletList.push_back(Eigen::Triplet<double>(colGlobId, rowGlobId,
        //                                              localStiffMatrix(i,
        //                                          StartGUI.sh    j)));
      }
    }
    // Сборка матрицы на основе триплетов с суммированием одинаковых индексов
    // globalStiffMatrix.setFromTriplets(tripletList.begin(),
    // tripletList.end());
  }
  emit calcFinished();
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

  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
  solver.compute(stiff); // Шаг 1: факторизация/анализ
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error("Solver info not success");
  }

  Eigen::VectorXd u = solver.solve(load); // Шаг 2: решение

  for (size_t i = 0; i < u.size(); i++) {
    qDebug() << i << " " << u[i] << "\n";
  }
}

// void Solver::updateProgress(QMessageBox *mes, int count) {
//   mes->setText(
//       QString("Creating local stiff matrix for element %1").arg(count));
// }

// void Solver::showResult(QMessageBox *mes) {
//   mes->setText(QString("Global stiff matrix sucsesfully created"));
// }