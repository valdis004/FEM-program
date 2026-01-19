#pragma once

// #include "load/load.h"
// #include <memory>
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/load/femload.h"
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/displacement/displacement.h" #include "load/femload.h"
// #include "displacement/displacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/point.h"
// #include "elementprovider.h"
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/displacement/displacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/femdisplacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/femload.h"
// #include "femtypes.h"
#include <QVector>
#include <qglobal.h>

// class NodeLoad;

class Node {
public:
  Point3 point;
  int dofCount;
  unsigned id;
  NodeLoad *nodeLoad{nullptr};
  NodeDisplacement *nodeDisplacement{nullptr};
  unsigned firstGlobStiffId;

  Node() = default;
  Node(Point3 point, int dofCount, int id)
      : point(point), dofCount(dofCount), id(id) {}

  ~Node() {
    if (!nodeLoad)
      delete nodeLoad;

    if (!nodeDisplacement)
      delete nodeDisplacement;
  }

  void setValues(Point3 point, int dofCount, int id) {
    this->point = point, this->dofCount = dofCount, this->id = id;
  }
};