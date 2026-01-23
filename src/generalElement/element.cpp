#include "element.h"

AbstractElement::AbstractElement(shared_ptr<AbstractLoad> load) {
  loads.push_back(load);
}
