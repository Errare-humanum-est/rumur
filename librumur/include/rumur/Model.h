#pragma once

#include <cstdint>
#include <iostream>
#include "location.hh"
#include <rumur/Decl.h>
#include <rumur/Function.h>
#include <rumur/Node.h>
#include <rumur/Rule.h>
#include <vector>

namespace rumur {

struct Model : public Node {

  std::vector<Decl*> decls;
  std::vector<Function*> functions;
  std::vector<Rule*> rules;

  Model() = delete;
  Model(std::vector<Decl*> &&decls_, std::vector<Function*> &&functions_,
    std::vector<Rule*> &&rules_, const location &loc_);
  Model(const Model &other);
  Model &operator=(Model other);
  friend void swap(Model &x, Model &y) noexcept;
  virtual ~Model();
  Model *clone() const final;

  // Get the size of the state data in bits.
  uint64_t size_bits() const;

  void generate(std::ostream &out) const;
  bool operator==(const Node &other) const final;

  // Get the number of (global or local) assumptions in the model.
  unsigned long assumption_count() const;

  // Update the bit offset of each variable declaration in the model.
  void reindex();
};

static inline std::ostream &operator<<(std::ostream &out, const Model &m) {
  m.generate(out);
  return out;
}

}
