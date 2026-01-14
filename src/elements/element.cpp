#include "element.h"
#include "plates/plates.h"
#include <Eigen/src/Core/Matrix.h>
#include <cstddef>
#include <exception>
#include <qexception.h>

#define SQRT_3 1.7320508075688772

QMap<ElementType, ElementData> ElementProvider::props;

void ElementProvider::initialize() {
  props.clear();
  ElementData MITC4MY = {
      4,
      {-1.0 / SQRT_3, 1.0 / SQRT_3},
      {-1.0 / SQRT_3, 1.0 / SQRT_3},
      {1.0, 1.0, 1.0, 1.0},
      4,
      true,
      3,
      {3, 3, 3, 3},
      {},
      {LoadType::FyMxMz, LoadType::FyMxMz, LoadType::FyMxMz, LoadType::FyMxMz},
      [](int index, const Point3 &point0, double step, double cosA,
         double sinA) {
        float x1 = point0.x;
        float y1 = point0.y;
        float z1 = point0.z;

        //   float x2 = x1 + step * cosA;
        //   float y2 = y1 + step;
        //   float z2 = z1 + step * sinA;

        switch (index) {
        case 0:
          return Point3(x1, y1, z1);
        case 1:
          return Point3(x1 + step * cosA, y1 + step, z1);
        case 2:
          return Point3(x1 + step * cosA, y1 + step, z1 + step * sinA);
        case 3:
          return Point3(x1, y1, z1 + step * sinA);
        default:
          throw std::exception();
        }
      }

  };
  props.insert(ElementType::MITC4MY, MITC4MY);
};

AbstractElement::AbstractElement(size_t id, const Node *nodes, ElementType type)
    : id(id), props(ElementProvider::props[type]), type(type) {
  for (size_t i = 0; i < props.nodesCount; i++) {
    this->nodes.push_back(nodes[i]);
  }
}

AbstractElement::AbstractElement(size_t id, const Node *nodes,
                                 const Material &material, ElementType type)
    : AbstractElement(id, nodes, type) {}

AbstractElement *AbstractElement::create(size_t id, ElementType type,
                                         const Node *nodes, void *ptr) {
  switch (type) {
  case ElementType::MITC4MY: {
    return new (ptr) MITC4PlateMy(id, nodes);
  }
  default:
    throw std::exception();
  }
}

void AbstractElement::setCalcProps(AbstractElement *ptr) {

  // Set load parameters
  auto props = ptr->props; // Properties of current element
  VectorXd coefs = ptr->getLoadVector();

  auto dofMap = props.dofMap;
  for (size_t i = 0; i < props.nodesCount; i++) {
    for (size_t j = 0; j < dofMap[i]; j++) {
    }
  }
}