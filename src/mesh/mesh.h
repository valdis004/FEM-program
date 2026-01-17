#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
// #include "../elements/elementprovider.h"
#include "../elements/element.h"
#include "../elements/elementprovider.h"
#include "../elements/point.h"
#include <QList>
#include <qglobal.h>

class Mesh {
private:
  QVector<AbstractElement *> elements;
  QVector<Node> nodes;

  bool isEqual(Point3 p1, Point3 p2);

  unsigned maxNodeIndexInList(const QList<Node> &list);

public:
  template <ElementType type> void createDefaultMesh() {
    // ElementProvider::initialize();

    auto data = ElementProvider<type>::data;
    AreaLoadQzMxMy load = AreaLoadQzMxMy(-100);

    float startx = 0;
    float starty = 0;
    float startz = 0;
    Point3 point00{startx, starty, startz};

    float step = 100;
    float lz = 1000;
    float lenghtPlate = 3000; // В мм
    int steps = (int)(lenghtPlate / step);
    int elementCount = lenghtPlate * lenghtPlate / (step * step);

    // Allocation for element array
    void *elementMemory = operator new(sizeof(AbstractElement) * elementCount);

    elements.reserve(elementCount);
    nodes.reserve(elementCount);

    float sinA = 0;
    float cosA = 1;

    for (int l = 0; l < steps; l++) {
      for (int k = 0; k < steps; k++) {

        Point3 point0{point00.x + l * step * cosA, point00.y + k * step,
                      point00.x + l * step * sinA};
        Node node;
        const int count = data.NODES_COUNT;
        Node nodesToElem[count];
        float checkValue = 0.01f;

        int elementsCount = 0;

        for (int j = 0; j < count; j++) {

          Point3 pointForNode =
              data.GET_POINT_FROM_INDEX_FN(j, point0, step, cosA, sinA);

          Node possibleNode;
          bool flag = false;
          if (j != 0) {
            // Проверка на то есть в этой точке уже нод или нет
            for (auto item : this->nodes) {
              if (isEqual(item.point, pointForNode)) {
                possibleNode = item;
                flag = true;
                break;
              }
            }
          }

          if (flag) {
            node = Node(possibleNode.point, possibleNode.dofCount,
                        possibleNode.id);
          } else {
            int index = 0;

            if (j != 0) {
              index = maxNodeIndexInList(this->nodes) + 1;
            }

            node = Node(pointForNode, data.DOF_COUNT, index);

            // Load standart sheme add
            // if (node.point.y == starty)
            //   node.nodeLoad = &load;

            // Add displ
            if (node.point.y == starty ||
                node.point.y == starty + lenghtPlate) {
              NodeDisplacementUzPsixPsiy disp =
                  NodeDisplacementUzPsixPsiy(true, true, true);
              node.nodeDisplacement = &disp;
            }
          }

          nodesToElem[j] = node;
          this->nodes.push_back(node);
        }

        void *ptr = static_cast<char *>(elementMemory) +
                    elementCount * sizeof(AbstractElement);

        this->elements.push_back(AbstractElement::create(
            elementsCount, type, nodesToElem, data.NODES_COUNT, ptr));
        elementsCount++;
      }
    }
  }

  void meshManager(ElementType type);
};