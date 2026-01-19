#include "load.h"
// #include "/home/vladislav/Документы/FEM/FEM
// program/src/elements/elementprovider.h" #include "femload.h"

AreaLoadQzMxMy::AreaLoadQzMxMy(double qz, double mx, double my)
    : qz(qz), mx(mx), my(my) {};

/*virtual*/ void AreaLoadQzMxMy::setValues(double *values) {
  values[0] = qz;
  values[1] = mx;
  values[2] = my;
}
// AreaLoad::AreaLoad(double qx, double qy, double qz) : AreaLoadPlate(qz) {
//   this->qx = qx;
//   this->qy = qy;
// }