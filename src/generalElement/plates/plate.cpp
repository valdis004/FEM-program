#include "plate.h"
#include "../load/load.h"
#include <memory>

/* virtual */ shared_ptr<AbstractLoad> Plate::createAndAddLoad() {
  shared_ptr<AreaLoadFzMxMy> load = std::make_shared<AreaLoadFzMxMy>();
  addLoad(load);
  return load;
}
