#pragma once

// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/elementdata.h"
#include "femtypes.h"
// #include "plates/plates.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/femdisplacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/femload.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"

template <ElementType type> struct ElementData {};

template <> struct ElementData<ElementType::MITC4MY> {
public:
#define SQRT_3 1.7320508075688772

  constexpr static short NODES_COUNT = 4;
  constexpr static short INT_POINTS_COUNT = 4;
  constexpr static short STIFF_MATRIX_SIZE = 12;
  constexpr static bool FULL_DOF = true;
  constexpr static short DOF_COUNT = 3;
  constexpr static short BAD_DOF_BEGIN = -1;
  constexpr static short BAD_DOF_COUNT = -1;

  constexpr static double XI_SET[INT_POINTS_COUNT] = {-1.0 / SQRT_3,
                                                      1.0 / SQRT_3};
  constexpr static double ETA_SET[INT_POINTS_COUNT] = {-1.0 / SQRT_3,
                                                       1.0 / SQRT_3};
  constexpr static double W_COEFS[INT_POINTS_COUNT] = {1.0, 1.0, 1.0, 1.0};

  constexpr static short DOF_MAP[] = {3, 3, 3, 3};
  constexpr static short BAD_DOF_MAP[NODES_COUNT] = {};
  constexpr static LoadType LOAD_MAP[] = {LoadType::FyMxMz, LoadType::FyMxMz,
                                          LoadType::FyMxMz, LoadType::FyMxMz};

  using MethodPtrDisp = NodeDisplacement *(*)(bool *value, Node *node);

  constexpr static MethodPtrDisp DISP_FN_MAP[NODES_COUNT] = {
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
      &NodeDisplacementUzPsixPsiy::create,
  };

  using MethodPtrLoad = NodeLoad *(*)(double *values, double *coefs);

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
          return Point3(x1 + step * cosA, y1, z1 + step * sinA);
        case 2:
          return Point3(x1 + step * cosA, y1 + step, z1 + step * sinA);
        case 3:
          return Point3(x1, y1 + step, z1);
        default:
          throw std::runtime_error("Invalid index");
        }
      };
};

template <ElementType type>
NodeLoad *createNodeLoadFromLoad(Load *load, double *coefs, int localNodeId) {

  using DATA = ElementData<type>;
  const int dof = DATA::DOF_COUNT;

  double values[dof];
  load->setValues(values);

  return DATA::LOAD_FN_MAP[localNodeId](values, coefs);
}

template <ElementType type>
NodeDisplacement *createNodeDisplacementFromDisplacement(Displacement *displ,
                                                         Node *node,
                                                         int locId) {
  using DATA = ElementData<type>;

  bool values[3];
  displ->setValues(
      values); // To get disp value from parameter Displacement *displ

  // Cretate for those parameteres node Disp
  return DATA::DISP_FN_MAP[locId](values, node);
}
//
//
//
//
//
template <ElementType type> static void setCalcProps(AbstractElement *ptr) {

  using DATA = ElementData<type>;
  // Set load parameters
  bool isLoad = ptr->generalLoad != nullptr;
  bool isDispl = ptr->generalDisp != nullptr;
  VectorXd coefs = ptr->getLoadVector();

  // Bad dof parameters
  short badDofBegin = DATA::BAD_DOF_BEGIN;
  short badDofCount = DATA::BAD_DOF_COUNT;
  short correcCount, correctFromPrevNode, corFromCurNode;
  bool isFullDof = DATA::FULL_DOF;

  auto dofMap = DATA::DOF_MAP;
  auto badDofMap = DATA::BAD_DOF_MAP;

  int countCoefs = 0;
  for (size_t i = 0; i < DATA::NODES_COUNT; i++) {
    const short curDof = dofMap[i];
    Node *currentNode = ptr->nodes[i];
    double currentCoefs[curDof];
    short id = currentNode->id;

    short correction = 0;
    if (!isFullDof) {
      correcCount = id % badDofBegin;
      correctFromPrevNode = correcCount * badDofCount;
      corFromCurNode = badDofMap[i];

      correction = correctFromPrevNode + corFromCurNode;
    }

    unsigned globalIndex = id * curDof - correction;
    currentNode->firstGlobStiffId = globalIndex;

    for (size_t j = 0; j < curDof; j++) {
      currentCoefs[j] = coefs[countCoefs++];
    }

    if (currentNode->nodeDisplacement) {
      currentNode->nodeDisplacement->setIndexesToZero(currentNode);
    }

    if (isLoad) {
      if (currentNode->nodeLoad) {
        currentNode->nodeLoad->appendValuesToNodeLoad(ptr->generalLoad,
                                                      currentCoefs);
      } else {
        currentNode->nodeLoad =
            createNodeLoadFromLoad<type>(ptr->generalLoad, currentCoefs, i);
      }
    }

    // if (isDispl) {
    //   currentNode->nodeDisplacement =
    //       createNodeDisplacementFromDisplacement<type>(ptr->generalDisp,
    //                                                    currentNode, i);
    // }
  }
}