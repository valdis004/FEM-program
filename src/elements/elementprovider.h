#pragma once

// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/elementdata.h"
#include "femtypes.h"
// #include "plates/plates.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/femdisplacement.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/femload.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"
#include <QMap>
#include <QVector>

#define SQRT_3 1.7320508075688772

using MethodPtrDisp = NodeDisplacement *(*)(bool *value, Node *node);
using MethodPtrLoad = NodeLoad *(*)(double *values, double *coefs);
using GetPointFunc = Point3 (*)(int index, Point3 &point0, double step,
                                double cosA, double sinA);

struct ElementData {
  short NODES_COUNT;
  short INT_POINTS_COUNT;
  short STIFF_MATRIX_SIZE;
  bool IS_FULL_DOF;
  short FULL_DOF_COUNT;
  short BAD_DOF_BEGIN; // С какого индекса надо начинат учитывать коррекцию от
                       // предыдущего элемента (для MITC16 = 16)
  short BAD_DOF_COUNT; // Количество степеней свободы, которое надо добавить
                       // чтобы получить fullDoff
  QVector<double> XI_SET;
  QVector<double> ETA_SET;
  QVector<double> W_COEFS;
  QVector<short> DOF_MAP;
  QVector<short> LOCAL_ID_FROM_STIFFMAT;
  QVector<short> BAD_DOF_MAP;
  QVector<LoadType> LOAD_MAP;

  QVector<MethodPtrDisp> DISP_FN_MAP;

  QVector<MethodPtrLoad> LOAD_FN_MAP;

  GetPointFunc GET_POINT_FROM_INDEX_FN;
};

class ElementProvider {
public:
  static QMap<ElementType, ElementData> elementData;

  static void init();
};