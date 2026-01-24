#include "element.h"
#include "femtypes.h"

AbstractElement::AbstractElement(shared_ptr<AbstractLoad> load,
                                 ElementType type) {
  this->type = type;
  loads.push_back(load);
}
