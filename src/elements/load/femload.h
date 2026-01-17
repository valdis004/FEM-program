#pragma once

// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"

class Node;

class NodeLoad {
public:
  double *values{nullptr};
  int countValues{0};
  //   double fx, fy, fz, mx, my, mz;

  virtual void setNodeLoadValues(double *values, double *coefs) = 0;

  virtual NodeLoad *create(double *values, double *coefs) = 0;

  virtual ~NodeLoad() = default;
};

class NodeLoadFzMxMy : public NodeLoad {
public:
  double fz, mx, my;

  virtual void setNodeLoadValues(double *values, double *coefs) override;

  virtual NodeLoad *create(double *values, double *coefs) override;
};

class NodeLoadFz : public NodeLoad {
public:
  double fz;

  virtual void setNodeLoadValues(double *values, double *coefs) override;

  virtual NodeLoad *create(double *values, double *coefs) override;
};

class NodeLoadMxMy : public NodeLoad {
public:
  double mx, my;

  virtual void setNodeLoadValues(double *values, double *coefs) override;

  virtual NodeLoad *create(double *values, double *coefs) override;
};