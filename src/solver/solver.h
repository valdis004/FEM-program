#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/elementprovider.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/mesh/mesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <QVector>

using Eigen::MatrixXd;
using Eigen::SparseMatrix;
using Eigen::SparseVector;
using Eigen::VectorXd;

class AbstractElement;

class Solver {
private:
  inline unsigned setParams(size_t i, const AbstractElement *element,
                            unsigned &correction, unsigned curDof,
                            unsigned localId, unsigned nodeId,
                            unsigned &fullDof);

  inline unsigned
  getGlobalIndexAndSetLoad(size_t i, const AbstractElement *element,
                           SparseVector<double> globalLoadVector);

  inline unsigned getGlobalIndex(size_t i, const AbstractElement *element);

  SparseMatrix<double> getGlobalStiffMatrix(const Mesh &mesh);

  SparseVector<double> getGlobalLoadVector(const Mesh &mesh);

  void applyBaundaryConditions(SparseMatrix<double> &globalMatrix,
                               const Mesh &mesh);

public:
  void calculate();
};