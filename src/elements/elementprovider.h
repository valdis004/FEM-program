#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/plates/plates.h"
#include "femtypes.h"
#include "load/femload.h"

template <typename T> struct ElementData {
  using ELEMENT_TYPE = T;

  constexpr static short NODES_COUNT = T::NODES_COUNT;
  constexpr static short INT_POINTS_COUNT = T::INT_POINTS_COUNT;
  constexpr static short STIFF_MATRIX_SIZE = T::STIFF_MATRIX_SIZE;
  constexpr static bool FULL_DOF = T::FULL_DOF;
  constexpr static short DOF_COUNT = T::DOF_COUNT;
  constexpr static short BAD_DOF_BEGIN = T::BAD_DOF_BEGIN;
  constexpr static short BAD_DOF_COUNT = T::BAD_DOF_COUNT;

  constexpr static double XI_SET[INT_POINTS_COUNT] = T::XI_SET;
  constexpr static double ETA_SET[INT_POINTS_COUNT] = T::ETA_SET;
  constexpr static double W_COEFS[INT_POINTS_COUNT] = T::W_COEFS;

  constexpr static short DOF_MAP[NODES_COUNT] = T::DOF_MAP;
  constexpr static short BAD_DOF_MAP[NODES_COUNT] = T::BAD_DOF_MAP;
  constexpr static LoadType LOAD_MAP[NODES_COUNT] = T::LOAD_MAP;
  constexpr static NodeDisplacement *(T::*DISP_FN_MAP[])(
      bool *value, Node *node) = T::DISP_FN_MAP;

  constexpr static NodeLoad *(T::*LOAD_FN_MAP[])(bool *value,
                                                 Node *node) = T::LOAD_FN_MAP;

  constexpr static Point3 (*GET_POINT_FROM_INDEX_FN)(
      int index, const Point3 &point0, double step, double cosA,
      double sinA) = T::GET_POINT_FROM_INDEX_FN;
};

template <ElementType t> class ElementProvider {};

template <> class ElementProvider<ElementType::MITC4MY> {
public:
  const constexpr static ElementData<MITC4PlateMy> data{};
};

template <ElementType t> class ElementProvider;

template <> class ElementProvider<ElementType::NONE> {
public:
  const constexpr static ElementData<MITC4PlateMy> data{};
};