#include <QVector>
#include <memory>
#include <qglobal.h>

#include "displacement/displacement.h"
#include "elements/femtypes.h"
#include "elements/point.h"
#include "load/load.h"

class Node;
class AbstractFemElement;

using std::shared_ptr;

class AbstractElement {
private:
  ElementType type;
  Point3 statrtPoint{0, 0, 0};
  QVector<shared_ptr<AbstractLoad>> loads;
  QVector<shared_ptr<AbstractDisplacement>> displacements;
  QVector<Node *> *nodes{nullptr};
  QVector<AbstractFemElement *> *elements{nullptr};

public:
  AbstractElement();

  AbstractElement(shared_ptr<AbstractLoad> load,
                  ElementType type = ElementType::NONE);

  inline void addLoad(shared_ptr<AbstractLoad> load) { loads.push_back(load); }

  inline short loadCount() const { return loads.size(); }

  virtual shared_ptr<AbstractLoad> createAndAddLoad() = 0;
};
