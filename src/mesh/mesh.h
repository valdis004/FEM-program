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
  QVector<AbstractElement *> elements{};
  QVector<Node *> nodes{};
  unsigned globaStiffMatrixSize = 0;
  unsigned tripletsCount = 0; // Колчиество узлов во всех элементах, считая
  // общие для Solver triplets аллокации

private:
  bool isEqual(const Point3 &p1, const Point3 &p2);

  unsigned maxNodeIndexInList(const QList<Node> &list);

public:
  void createDefaultMesh(ElementType type, QMessageBox *mes);

  void meshManager(QMessageBox *mes, ElementType type);

  ~Mesh();

signals:
  void progressChanged(QMessageBox *mes, int count);
  void meshFinished(QMessageBox *mes, int count);
};