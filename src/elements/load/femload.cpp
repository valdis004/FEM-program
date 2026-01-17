#include "femload.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"
// #include <exception>
// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
// #include <stdexcept>

/*virtual */ void NodeLoadFzMxMy::setNodeLoadValues(double *value,
                                                    double *coefs) {
  fz = value[0] * coefs[0];
  mx = value[1] * coefs[1];
  my = value[2] * coefs[2];
};

/*virtual*/ NodeLoad *NodeLoadFzMxMy::create(double *values, double *coefs) {
  NodeLoad *nodeLoad = new NodeLoadFzMxMy();
  nodeLoad->setNodeLoadValues(values, coefs);

  values = new double[3]{fz, mx, my};
  countValues = 3;
  return nodeLoad;
}

/*virtual */ void NodeLoadFz::setNodeLoadValues(double *value, double *coefs) {
  fz = value[0] * coefs[0];
};

/*virtual*/ NodeLoad *NodeLoadFz::create(double *values, double *coefs) {
  NodeLoad *nodeLoad = new NodeLoadFz();
  nodeLoad->setNodeLoadValues(values, coefs);

  values = new double[1]{fz};
  countValues = 1;
  return nodeLoad;
}

/*virtual */ void NodeLoadMxMy::setNodeLoadValues(double *value,
                                                  double *coefs) {
  mx = value[1] * coefs[1];
  my = value[2] * coefs[1];
};

/*virtual*/ NodeLoad *NodeLoadMxMy::create(double *values, double *coefs) {
  NodeLoad *nodeLoad = new NodeLoadFzMxMy();
  nodeLoad->setNodeLoadValues(values, coefs);

  values = new double[2]{mx, my};
  countValues = 2;
  return nodeLoad;
}