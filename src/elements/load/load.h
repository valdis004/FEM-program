#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"

#include "femload.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"

class NodeLoad;

class Load {
public:
  virtual void setValues(double *values) = 0;
};

class AreaLoadQzMxMy : public Load {
private:
  double qz, mx, my;

public:
  AreaLoadQzMxMy(double qz = 0, double mx = 0, double my = 0);

  virtual void setValues(double *values) override;
};

class AreaLoad : public Load {
private:
  double qx, qy, qz, mx, my, mz;

public:
  AreaLoad(double qx = 0, double qy = 0, double qz = 0);
};