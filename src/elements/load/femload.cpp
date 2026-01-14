#include "femload.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"
#include <exception>

/*virtual */ void NodeLoadPlateFull::setValues(double *value, double *coefs) {
  fz = value[0] * coefs[0];
  mx = value[1] * coefs[1];
  my = value[2] * coefs[2];
};

/*virtual */ void NodeLoadPlateFz::setValues(double *value, double *coefs) {
  fz = value[0] * coefs[0];
};

/*virtual */ void NodeLoadPlateMxMy::setValues(double *value, double *coefs) {
  mx = value[1] * coefs[1];
  my = value[2] * coefs[1];
};

NodeLoad *NodeLoad::create(LoadType type, double *value, double *coefs) {
  switch (type) {
  case LoadType::Fx: {
    auto var = new NodeLoadPlateFz();
    var->setValues(value, coefs);
    return var;
  }
  default:
    throw std::exception();
  }
}