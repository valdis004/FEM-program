#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"

class NodeLoad {
public:
  //   double fx, fy, fz, mx, my, mz;

  virtual void setValues(double *value, double *coefs) = 0;

  static NodeLoad *create(LoadType type, double *value, double *coefs);
};

class NodeLoadPlateFull : public NodeLoad {
public:
  double fz, mx, my;

  virtual void setValues(double *value, double *coefs) override;
};

class NodeLoadPlateFz : public NodeLoad {
public:
  double fz;

  virtual void setValues(double *value, double *coefs) override;
};

class NodeLoadPlateMxMy : public NodeLoad {
public:
  double mx, my;

  virtual void setValues(double *value, double *coefs) override;
};