#include "element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
// #include "elementprovider.h"
// #include "load/load.h"
#include "plates/plates.h"
#include <Eigen/src/Core/Matrix.h>
#include <cstddef>
#include <exception>
#include <qexception.h>

AbstractElement::AbstractElement(size_t id, Node **nodes, int count,
                                 ElementType type)
    : id(id), type(type) {
  for (size_t i = 0; i < count; i++) {
    this->nodes.push_back(nodes[i]);
  }
  nodesCount = count;
}

AbstractElement::AbstractElement(size_t id, Node **nodes, int count,
                                 const Material &material, ElementType type)
    : AbstractElement(id, nodes, count, type) {}

AbstractElement *AbstractElement::create(size_t id, ElementType type,
                                         Node **nodes, int count) {
  switch (type) {
  case ElementType::MITC4MY: {
    return new MITC4PlateMy(id, nodes);
  }
  default:
    throw std::exception();
  }
}