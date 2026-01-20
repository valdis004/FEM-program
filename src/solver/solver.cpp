#include "solver.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include <Eigen/src/SparseCore/SparseVector.h>
#include <QVector>
#include <cstddef>
#include <qglobal.h>

unsigned Solver::setParams(size_t i, const AbstractElement *element,
                           unsigned &correction, unsigned curDof,
                           unsigned localId, unsigned nodeId,
                           unsigned &fullDof) {
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

SparseMatrix<double> Solver::getGlobalStiffMatrix(const Mesh &mesh) {
  unsigned globalMatrixSize = mesh.globaStiffMatrixSize;
  SparseMatrix<double> globalStiffMatrix(globalMatrixSize, globalMatrixSize);
  SparseVector<double> globalLoadVector(globalMatrixSize);

  auto elements = mesh.elements;
  for (AbstractElement *element : elements) {
    ElementData data = element->data;
    QVector<Node *> nodes = element->nodes;
    MatrixXd localStiffMatrix = element->getLocalStiffMatrix();

    QVector<Eigen::Triplet<double>> tripletList;
    tripletList.reserve(data.STIFF_MATRIX_SIZE * data.STIFF_MATRIX_SIZE);

    for (size_t i = 0; i < data.STIFF_MATRIX_SIZE; i++) {
      unsigned colGlobId =
          getGlobalIndexAndSetLoad(i, element, globalLoadVector);
      for (size_t j = 0; j < data.STIFF_MATRIX_SIZE; j++) {
        unsigned rowGlobId = getGlobalIndex(i, element);
        globalStiffMatrix.coeffRef(rowGlobId, colGlobId) +=
            localStiffMatrix(i, j);

        // tripletList.push_back(Eigen::Triplet<double>(colGlobId, rowGlobId,
        //                                              localStiffMatrix(i,
        //                                              j)));
      }
    }
    // Сборка матрицы на основе триплетов с суммированием одинаковых индексов
    // globalStiffMatrix.setFromTriplets(tripletList.begin(),
    // tripletList.end());
  }
  return globalStiffMatrix;
}