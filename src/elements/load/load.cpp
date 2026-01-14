#include "load.h"

AreaLoadPlate::AreaLoadPlate(double qz, double mx, double my)
    : qz(qz), mx(mx), my(my) {};

/*virtual*/ void AreaLoadPlate::setValues(double *values) {
  values[0] = qz;
  values[1] = mx;
  values[2] = my;
}
// AreaLoad::AreaLoad(double qx, double qy, double qz) : AreaLoadPlate(qz) {
//   this->qx = qx;
//   this->qy = qy;
// }