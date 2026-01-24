#include <QList>
#include <QtAlgorithms>
// #include <stdexcept>

// #include <new>

// #include "../elements/element.h"
#include "../elements/elementprovider.h"
// #include "../elements/load/load.h"
#include "../generalElement/displacement/displacement.h"
#include "mesh.h"

Mesh::~Mesh() {
  for (auto node : nodes) {
    delete node;
  }

  for (auto element : elements) {
    delete element;
  }
}

bool Mesh::isEqual(const Point3 &p1, const Point3 &p2) {
  return abs(p1.x - p2.x) < 0.001 && abs(p1.y - p2.y) < 0.001 &&
         abs(p1.y - p2.y) < 0.001;
}

unsigned Mesh::maxNodeIndexInList(const QList<Node> &list) {
  if (list.empty())
    throw std::exception();

  int maxIndex = list.first().id;
  for (auto item : list) {
    if (item.id > maxIndex)
      maxIndex = item.id;
  }
  return maxIndex;
}

void Mesh::createDefaultMesh(ElementType type) {
  // ElementProvider::initialize();

  auto DATA = ElementProvider::elementData[type];
  double loadv[] = {-100, 0, 0};
  AbstractLoad *load = new AreaLoadFzMxMy(loadv, 3);

  float startx = 0;
  float starty = 0;
  float startz = 0;
  Point3 point00{startx, starty, startz};

  float step = 200;
  float lenghtPlate = 2000; // В мм
  int steps = (int)(lenghtPlate / step);
  int elementCount = lenghtPlate * lenghtPlate / (step * step);

  this->elements.reserve(elements.size() + elementCount);
  this->nodes.reserve(nodes.size() + elementCount * 20);

  float sinA = 0;
  float cosA = 1;

  int crtdElmtsCnt = 0;
  int crtdNdsCnt = 0;

  for (int l = 0; l < steps; l++) {
    for (int k = 0; k < steps; k++) {

      Point3 point0{point00.x + l * step * cosA, point00.y + k * step,
                    point00.x + l * step * sinA};
      const int ndsCntElm = DATA.NODES_COUNT;
      Node *nodesToElem[ndsCntElm];
      float checkValue = 0.01f;

      for (int j = 0; j < ndsCntElm; j++) {

        Point3 pointForNode =
            DATA.GET_POINT_FROM_INDEX_FN(j, point0, step, cosA, sinA);

        // Проверка на то есть в этой точке уже нод или нет
        Node *node;
        for (auto item : nodes) {
          if (isEqual(item->point, pointForNode)) {

            node = item;
            goto nodeAlreadyExists;
          }
        }

        node = new Node(pointForNode, DATA.FULL_DOF_COUNT, crtdNdsCnt++,
                        DATA.OUTPUT_VALUES_COUNT);

        // Add displ
        if (node->point.x == startx || node->point.x == startx + lenghtPlate) {
          NodeDisplacementUzPsixPsiy *disp =
              new NodeDisplacementUzPsixPsiy(true, true, true);
          node->nodeDisplacement = disp;
        }

        this->nodes.push_back(node);
      nodeAlreadyExists:
        nodesToElem[j] = node;
      }

      auto element = AbstractFemElement::create(crtdElmtsCnt++, type,
                                                nodesToElem, DATA.NODES_COUNT);
      element->setLoad(load);
      AbstractFemElement::setCalcProps(element, globaStiffMatrixSize);
      this->elements.push_back(element);

      emit progressChanged(crtdElmtsCnt);
    }
  }

  tripletsCount +=
      elementCount * DATA.STIFF_MATRIX_SIZE * DATA.STIFF_MATRIX_SIZE;

  emit meshFinished(elementCount);
}
