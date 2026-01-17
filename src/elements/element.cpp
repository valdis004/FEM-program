#include "element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
#include "elementprovider.h"
#include "load/load.h"
#include "plates/plates.h"
#include <Eigen/src/Core/Matrix.h>
#include <cstddef>
#include <exception>
#include <qexception.h>

AbstractElement::AbstractElement(size_t id, const Node *nodes, int count,
                                 ElementType type)
    : id(id), type(type) {
  for (size_t i = 0; i < count; i++) {
    this->nodes.push_back(nodes[i]);
  }
}

AbstractElement::AbstractElement(size_t id, const Node *nodes, int count,
                                 const Material &material, ElementType type)
    : AbstractElement(id, nodes, count, type) {}

AbstractElement *AbstractElement::create(size_t id, ElementType type,
                                         const Node *nodes, int count,
                                         void *ptr) {
  switch (type) {
  case ElementType::MITC4MY: {
    return new (ptr) MITC4PlateMy(id, nodes);
  }
  default:
    throw std::exception();
  }
}

template <ElementType type>
void AbstractElement::setCalcProps(AbstractElement *ptr) {

  // Set load parameters
  auto props = ElementProvider<type>::data; // Properties of current element
  bool isLoad = ptr->generalLoad != nullptr;
  bool isDispl = ptr->generalDisp != nullptr;
  VectorXd coefs = ptr->getLoadVector();

  // Bad dof parameters
  short badDofBegin = props.BAD_DOF_BEGIN;
  short badDofCount = props.BAD_DOF_COUNT;
  short correcCount, correctFromPrevNode, corFromCurNode;
  bool fullDof = props.FULL_DOF;
  auto T = props.ELEMENT_TYPE;

  auto dofMap = props.DOF_MAP;
  auto badDofMap = props.BAD_DOF_MAP;
  auto loadMap = props.LOAD_MAP;

  int countCoefs = 0;
  for (size_t i = 0; i < props.nodesCount; i++) {
    Node currentNode = ptr->nodes[i];
    double currentCoefs[dofMap[i]];
    DisplType dispType = loadMap[i];
    short id = currentNode.id;
    auto dispFn = props.LOAD_FN_MAP[i];

    short correction = 0;
    if (!fullDof) {
      correcCount = id % badDofBegin;
      correctFromPrevNode = correcCount * badDofCount;
      corFromCurNode = badDofMap[i];

      correction = correctFromPrevNode + corFromCurNode;
    }

    unsigned globalIndex = id * fullDof - correction;
    currentNode.firstGlobId = globalIndex;

    for (size_t j = 0; j < dofMap[i]; j++) {
      currentCoefs[j] = coefs[countCoefs++];
    }

    if (currentNode.nodeDisplacement) {
      currentNode.nodeDisplacement->setIndexesToZero(&currentNode);
    }

    if (isLoad) {
      currentNode.nodeLoad =
          Load::createNodeLoadFromLoad<type>(ptr->generalLoad, currentCoefs, i);
    }
    if (isDispl) {
      currentNode.nodeDisplacement =
          Displacement::createNodeDisplacementFromDisplacement(
              dispType, ptr->generalDisp, currentNode, dispFn);
    }
  }
}