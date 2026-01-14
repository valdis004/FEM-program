#pragma once

class Load {
public:
  virtual void setValues(double *values) = 0;

  // virtual void setNodeLoadFromLoad(Load *load, ElementType type,
  //                                  Node *nodes) = 0;
};

class AreaLoadPlate : public Load {
private:
  double qz, mx, my;

public:
  AreaLoadPlate(double qz = 0, double mx = 0, double my = 0);

  virtual void setValues(double *values) override;
};

class AreaLoad : public Load {
private:
  double qx, qy;

public:
  AreaLoad(double qx = 0, double qy = 0, double qz = 0);
};