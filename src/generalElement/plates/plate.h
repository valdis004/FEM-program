#include "../element.h"

class Plate : public AbstractElement {
  virtual shared_ptr<AbstractLoad> createAndAddLoad() override;
};