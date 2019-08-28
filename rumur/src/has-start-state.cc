#include <cstddef>
#include "has-start-state.h"
#include <rumur/rumur.h>

using namespace rumur;

namespace { class Finder : public ConstTraversal {

 public:
  bool result = false;

  void visit_startstate(const StartState&) final {
    result = true;
  }

  virtual ~Finder() = default;
}; }

bool has_start_state(const Model &m) {
  Finder f;
  f.dispatch(m);
  return f.result;
}
