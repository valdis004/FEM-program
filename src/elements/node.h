#pragma once

// #include "load/load.h"
// #include <memory>
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/load/femload.h" #include "load/femload.h"
class NodeLoad;

struct Point3 {
  float x, y, z;
};

class Node {
public:
  Point3 point;
  int dofCount;
  unsigned id;
  NodeLoad *load;

  Node() = default;
  Node(Point3 point, int dofCount, int id)
      : point(point), dofCount(dofCount), id(id) {}
};