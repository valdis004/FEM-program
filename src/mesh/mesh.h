#pragma once

#include "../elements/element.h"
#include "../elements/node.h"
#include <QList>
#include <qglobal.h>

class Mesh {
private:
  QVector<AbstractElement *> elements;
  QVector<Node> nodes;

  bool isEqual(Point3 p1, Point3 p2);

  unsigned maxNodeIndexInList(const QList<Node> &list);

public:
  void createDefaultMesh();
};