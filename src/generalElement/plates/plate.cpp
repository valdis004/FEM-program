#include <memory>

#include "../load/load.h"
#include "femtypes.h"
#include "plate.h"

Plate::Plate(shared_ptr<AbstractLoad> load, ElementType type)
    : AbstractElement(load, type) {}

/* virtual */ shared_ptr<AbstractLoad> Plate::createAndAddLoad() {
  shared_ptr<AreaLoadFzMxMy> load = std::make_shared<AreaLoadFzMxMy>();
  addLoad(load);
  return load;
}
