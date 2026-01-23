#include "displacement/displacement.h"
#include "load/load.h"
#include <QVector>
// #include <cstddef>
#include <memory>
#include <qglobal.h>

using std::shared_ptr;

class AbstractElement {
private:
  QVector<shared_ptr<AbstractLoad>> loads;
  QVector<shared_ptr<AbstractDisplacement>> displacements;

public:
  AbstractElement();

  AbstractElement(shared_ptr<AbstractLoad> load);

  inline void addLoad(shared_ptr<AbstractLoad> load) { loads.push_back(load); }

  inline short loadCount() const { return loads.size(); }

  virtual shared_ptr<AbstractLoad> createAndAddLoad() = 0;
};
