#pragma once

#include "../element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"

// #include "../material/PlateMaterial.h"

class MITC4PlateMy : public AbstractElement {
private:
  MatrixXd jMatrix(double xi, double eta);

  MatrixXd cMatrix(int type);

  MatrixXd integrateingFn(double xi, double eta, int type);

  MatrixXd bMatrix(double xi, double eta, int type);

public:
  MITC4PlateMy(size_t id, const Node *nodes, int count = NODES_COUNT);

  MITC4PlateMy(size_t id, const Node *nodes, const Material &material,
               int count = NODES_COUNT);

  virtual MatrixXd getLocalStiffMatrix() override;

  virtual VectorXd getLoadVector() override;

  virtual VectorXd getResultVector(VectorXd U, double xi, int index) override;

#define SQRT_3 1.7320508075688772

  constexpr static short NODES_COUNT = 4;
  constexpr static short INT_POINTS_COUNT = 4;
  constexpr static short STIFF_MATRIX_SIZE = 12;
  constexpr static bool FULL_DOF = true;
  constexpr static short DOF_COUNT = 3;
  constexpr static short BAD_DOF_BEGIN = 16;
  constexpr static short BAD_DOF_COUNT = 6;

  constexpr static double XI_SET[INT_POINTS_COUNT] = {-1.0 / SQRT_3,
                                                      1.0 / SQRT_3};
  constexpr static double ETA_SET[INT_POINTS_COUNT] = {-1.0 / SQRT_3,
                                                       1.0 / SQRT_3};
  constexpr static double W_COEFS[INT_POINTS_COUNT] = {1.0, 1.0, 1.0, 1.0};

  constexpr static short DOF_MAP[NODES_COUNT] = {3, 3, 3, 3};
  constexpr static short BAD_DOF_MAP[NODES_COUNT] = {};
  constexpr static LoadType LOAD_MAP[NODES_COUNT] = {
      LoadType::FyMxMz, LoadType::FyMxMz, LoadType::FyMxMz, LoadType::FyMxMz};

  using MethodPtrDisp = NodeDisplacement *(
      NodeDisplacementUzPsixPsiy::*)(bool *value, Node *node);

  constexpr static MethodPtrDisp DISP_FN_MAP[NODES_COUNT] = {
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
  };

  using MethodPtrLoad = NodeLoad *(NodeLoadFzMxMy::*)(double *values,
                                                      double *coefs);

  constexpr static MethodPtrLoad LOAD_FN_MAP[NODES_COUNT] = {
      &NodeLoadFzMxMy::create,
      &NodeLoadFzMxMy::create,
      &NodeLoadFzMxMy::create,
      &NodeLoadFzMxMy::create,
  };

  constexpr static Point3 (*GET_POINT_FROM_INDEX_FN)(int index,
                                                     const Point3 &point0,
                                                     double step, double cosA,
                                                     double sinA) =
      [](int index, const Point3 &point0, double step, double cosA,
         double sinA) {
        float x1 = point0.x;
        float y1 = point0.y;
        float z1 = point0.z;

        //   float x2 = x1 + step * cosA;
        //   float y2 = y1 + step;
        //   float z2 = z1 + step * sinA;

        switch (index) {
        case 0:
          return Point3(x1, y1, z1);
        case 1:
          return Point3(x1 + step * cosA, y1 + step, z1);
        case 2:
          return Point3(x1 + step * cosA, y1 + step, z1 + step * sinA);
        case 3:
          return Point3(x1, y1, z1 + step * sinA);
        default:
          throw std::exception();
        }
      };
};