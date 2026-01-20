#pragma once

// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/elementprovider.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/mesh/mesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <QVector>
#include <utility>

using Eigen::MatrixXd;
using Eigen::SparseMatrix;
using Eigen::SparseVector;
using Eigen::VectorXd;

class AbstractElement;

class Solver : public QObject {
  Q_OBJECT
private:
  unsigned globalMatrixSize = 0;

  inline void setParams(size_t i, const AbstractElement *element,
                        unsigned &correction, unsigned &curDof,
                        unsigned &localId, unsigned &nodeId, unsigned &fullDof);

  inline unsigned
  getGlobalIndexAndSetLoad(size_t i, const AbstractElement *element,
                           SparseVector<double> globalLoadVector);

  inline unsigned getGlobalIndex(size_t i, const AbstractElement *element);

  std::pair<SparseMatrix<double>, SparseVector<double>>
  getGlobalStiffMatrixAndLoadVector(Mesh *mesh);

  SparseVector<double> getGlobalLoadVector(Mesh *mesh);

  void applyBaundaryConditions(SparseMatrix<double> &globalMatrix,
                               SparseVector<double> &globalVector, Mesh *mesh);

public:
  void calculate(Mesh *mesh);

signals:
  void progressChanged(unsigned count);
  void calcFinished();

  // public slots:
  //   void updateProgress(QMessageBox *mes, int count);
  //   void showResult(QMessageBox *mes);
};