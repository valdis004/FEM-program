#pragma once

#include "../element.h"

// #include "../material/PlateMaterial.h"

class MITC4PlateMy : public AbstractElement {
private:
  MatrixXd jMatrix(double xi, double eta);

  MatrixXd cMatrix(int type);

  MatrixXd integrateingFn(double xi, double eta, int type);

  MatrixXd bMatrix(double xi, double eta, int type);

public:
  MITC4PlateMy(size_t id, const Node *nodes);

  MITC4PlateMy(size_t id, const Node *nodes, const Material &material);

  virtual MatrixXd getLocalStiffMatrix() override;

  virtual VectorXd getLoadVector() override;

  virtual VectorXd getResultVector(VectorXd U, double xi, int index) override;
};