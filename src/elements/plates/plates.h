#pragma once

#include "../element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/elementprovider.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"
#include <stdexcept>

// #include "../material/PlateMaterial.h"

class MITC4PlateMy : public AbstractElement {
private:
  MatrixXd jMatrix(double xi, double eta);

  MatrixXd cMatrix(int type);

  MatrixXd integrateingFn(double xi, double eta, int type);

  MatrixXd bMatrix(double xi, double eta, int type);

public:
  MITC4PlateMy(size_t id, Node **nodes, int count = 4);

  MITC4PlateMy(size_t id, Node **nodes, const Material &material,
               int count = 4);

  virtual MatrixXd getLocalStiffMatrix() override;

  virtual VectorXd getLoadVector() override;

  virtual VectorXd getResultVector(VectorXd U, double xi, int index) override;
};