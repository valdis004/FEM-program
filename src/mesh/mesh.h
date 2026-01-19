#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
// #include "../elements/elementprovider.h"
#include "../elements/element.h"
#include "../elements/elementprovider.h"
#include "../elements/point.h"
#include <QList>
#include <QMessageBox>
#include <QObject>
// #include <cstddef>
#include <qglobal.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <vector>

class Mesh : public QObject {
  Q_OBJECT
public:
  std::vector<AbstractElement *> elements{};
  std::vector<Node *> nodes{};

private:
  bool isEqual(const Point3 &p1, const Point3 &p2);

  unsigned maxNodeIndexInList(const QList<Node> &list);

public:
  template <ElementType type> void createDefaultMesh(QMessageBox *mes) {
    // ElementProvider::initialize();

    using DATA = ElementData<type>;
    Load *load = new AreaLoadQzMxMy(-100);

    float startx = 0;
    float starty = 0;
    float startz = 0;
    Point3 point00{startx, starty, startz};

    float step = 250;
    float lenghtPlate = 3000; // В мм
    int steps = (int)(lenghtPlate / step);
    int elementCount = lenghtPlate * lenghtPlate / (step * step);

    this->elements.reserve(elementCount);
    this->nodes.reserve(elementCount * 20);

    float sinA = 0;
    float cosA = 1;

    int crtdElmtsCnt = 0;
    int crtdNdsCnt = 0;

    for (int l = 0; l < steps; l++) {
      for (int k = 0; k < steps; k++) {

        Point3 point0{point00.x + l * step * cosA, point00.y + k * step,
                      point00.x + l * step * sinA};
        const int ndsCntElm = DATA::NODES_COUNT;
        Node *nodesToElem[ndsCntElm];
        float checkValue = 0.01f;

        for (int j = 0; j < ndsCntElm; j++) {

          Point3 pointForNode =
              DATA::GET_POINT_FROM_INDEX_FN(j, point0, step, cosA, sinA);

          // Проверка на то есть в этой точке уже нод или нет
          Node *node;
          for (auto item : nodes) {
            if (isEqual(item->point, pointForNode)) {

              node = item;
              goto nodeAlreadyExists;
            }
          }

          node = new Node(pointForNode, DATA::DOF_COUNT, crtdNdsCnt++);

          // Add displ
          if (node->point.x == startx ||
              node->point.x == startx + lenghtPlate) {
            NodeDisplacementUzPsixPsiy *disp =
                new NodeDisplacementUzPsixPsiy(true, true, true);
            node->nodeDisplacement = disp;
          }

          this->nodes.push_back(node);
        nodeAlreadyExists:
          nodesToElem[j] = node;
        }

        auto element = AbstractElement::create(crtdElmtsCnt++, type,
                                               nodesToElem, DATA::NODES_COUNT);
        element->setLoad(load);
        this->elements.push_back(element);
        setCalcProps<type>(element);

        emit progressChanged(mes, crtdElmtsCnt);
      }
    }

    emit meshFinished(mes, elementCount);
  }

  void meshManager(QMessageBox *mes, ElementType type);

signals:
  void progressChanged(QMessageBox *mes, int count);
  void meshFinished(QMessageBox *mes, int count);
};