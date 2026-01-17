
#include "displacement.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/femdisplacement.h"
// #include <stdexcept>

// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/load/load.h"

/*static*/ NodeDisplacement *
Displacement::createNodeDisplacementFromDisplacement(
    DisplType disType, Displacement *displ, Node *node,
    NodeDisplacement *(*createFn)(bool *value, Node *node)) {
  bool values[3];
  displ->setValues(
      values); // To get disp value from parameter Displacement *displ

  // Cretate for those parameteres node Disp
  return createFn(values, node);
}

DisplacementUzPsixPsiy::DisplacementUzPsixPsiy(bool u_z, bool psi_x, bool psi_y)
    : u_z(u_z), psi_x(psi_x), psi_y(psi_y) {}

/*virtual*/ void DisplacementUzPsixPsiy::setValues(bool *values) {
  values[0] = u_z;
  values[1] = psi_x;
  values[2] = psi_y;
}