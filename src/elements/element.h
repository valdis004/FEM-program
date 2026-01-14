#pragma once

#include "material/material.h"
#include "node.h"
// #include "plates/plates.h"
// #include "plates/plates.h"
#include <Eigen/Dense>
#include <QVector>
// #include <exception>
#include <cstddef>
#include <qglobal.h>
#include <qmap.h>
#include <qnamespace.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;

enum ElementType { MITC4MY, MITC9, MITC16, NONE };

enum DisplType // Тип степеней свободы, который поддерживает узел
{
  Ux,
  Uy,
  UxUy,
  RUy,
  RUxUy,
  RxRyUy
};

enum LoadType {
  Fx,
  Fy,
  FxFy,
  MFy,
  MFx,
  MFxy,
  q,
  FyMxMz,
  MxMz,
};

// enum OutputType {
//   Ux = 0,
//   Uy = 1,
//   Theta = 2,
//   Mx = 3,
//   Qx = 4,
//   Nx = 5,
//   My = 6,

//   Mxy = 7,
//   Qy = 8,
//   None = 9
// };

//

struct ElementData {
  // Gauss integration
  short gaussPointsCount;
  QVector<double> xiSet;
  QVector<double> etaSet;
  QVector<double> wCoefs;

  // Nodes
  short nodesCount;
  bool fullDof;
  int dofCount;
  QVector<short> dofMap;
  QVector<short> badDofMap;
  QVector<LoadType> loadMap;

  Point3 (*getPointFromIndex)(int index, const Point3 &point0, double step,
                              double cosA, double sinA);
  // // funcs
  // MatrixXd (*localStiffMatrixFn)();
  // VectorXd (*loadVectorFn)();
  // VectorXd (*resultVectorFn)(VectorXd U, double xi, int index);
};

class ElementProvider {
public:
  static QMap<ElementType, ElementData> props;

  static void initialize();
};

class AbstractElement {
protected:
  ElementData &props;
  ElementType type;
  QVector<Node> nodes;
  size_t id;

public:
  AbstractElement(size_t id, const Node *nodes,
                  ElementType type = ElementType::NONE);

  AbstractElement(size_t id, const Node *nodes, const Material &material,
                  ElementType type = ElementType::NONE);

  virtual MatrixXd getLocalStiffMatrix() = 0;

  virtual VectorXd getLoadVector() = 0;

  virtual VectorXd getResultVector(VectorXd U, double xi, int index) = 0;

  virtual ~AbstractElement() = default;

  static AbstractElement *create(size_t id, ElementType type, const Node *nodes,
                                 void *ptr);

  static void setCalcProps(AbstractElement *ptr);
};