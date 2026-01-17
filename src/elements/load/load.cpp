#include "load.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/elementprovider.h"
#include "femload.h"

template <ElementType type>
NodeLoad *Load::createNodeLoadFromLoad(Load *load, double *coefs,
                                       int localNodeId) {

  const int dof = ElementProvider<type>::data.DOF_COUNT;
  auto fn = ElementProvider<type>::data.LOAD_FN_MAP[localNodeId];

  double values[dof];
  load->setValues(values);

  return fn(values, coefs);
}

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