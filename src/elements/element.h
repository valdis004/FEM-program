#pragma once

// #include "displacement/displacement.h"
// #include "displacement/displacement.h"
// #include "elementprovider.h
#include "elementprovider.h"
#include "material/material.h"
// #include "node.h"
// #include "plates/plates.h"
// #include "plates/plates.h"
#include <Eigen/Dense>
#include <QVector>
// #include <exception>
// #include "elementprovider.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/elementprovider.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"
#include "femtypes.h"
#include <cstddef>
#include <qglobal.h>
#include <qmap.h>
#include <qnamespace.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class AbstractElement {
private:
  // const ElementData &check(ElementType type);

public:
  const ElementType type;
  ElementData &data;
  QVector<Node *> nodes;
  unsigned nodesCount;
  size_t id;
  Load *generalLoad;
  Displacement *generalDisp;

public:
  AbstractElement(size_t id, Node **nodes, int count,
                  ElementType type = ElementType::NONE);

  AbstractElement(size_t id, Node **nodes, int count, const Material &material,
                  ElementType type = ElementType::NONE);

  virtual MatrixXd getLocalStiffMatrix() = 0;

  virtual VectorXd getLoadVector() = 0;

  virtual VectorXd getResultVector(VectorXd U, double xi, int index) = 0;

  virtual ~AbstractElement() = default;

  static AbstractElement *create(size_t id, ElementType type, Node **nodes,
                                 int count);

  static void setCalcProps(AbstractElement *ptr, unsigned &globalMatrixSize);

  void setLoad(Load *load) { this->generalLoad = load; }

  void setDisp(Displacement *disp) { this->generalDisp = disp; }
};