#include "element.h"
#include "femtypes.h"
#include "meshdata.h"
#include <memory>

AbstractElement::AbstractElement(shared_ptr<AbstractLoad> load,
                                 ElementType type, unsigned lenght) {
  this->lenght = lenght;
  this->type = type;
  loads.push_back(load);
  meshData = std::make_shared<MeshData>();
}

double AbstractElement::getLenght() const { return lenght; }

ElementType AbstractElement::getType() const { return type; }

Point3 AbstractElement::getStartPoint() const { return statrtPoint; };

void AbstractElement::addLoad(shared_ptr<AbstractLoad> load) {
  loads.push_back(load);
}

inline short AbstractElement::loadCount() const { return loads.size(); }
