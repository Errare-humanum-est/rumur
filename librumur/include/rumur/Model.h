#pragma once

#include <cstdint>
#include <gmpxx.h>
#include <iostream>
#include "location.hh"
#include <memory>
#include <rumur/Decl.h>
#include <rumur/Function.h>
#include <rumur/Node.h>
#include <rumur/Rule.h>
#include <vector>

namespace rumur {

struct Model : public Node {

  std::vector<std::shared_ptr<Decl>> decls;
  std::vector<std::shared_ptr<Function>> functions;
  std::vector<std::shared_ptr<Rule>> rules;

  Model() = delete;
  Model(std::vector<std::shared_ptr<Decl>> &&decls_,
    std::vector<std::shared_ptr<Function>> &&functions_,
    std::vector<std::shared_ptr<Rule>> &&rules_, const location &loc_);
  Model(const Model &other);
  Model &operator=(Model other);
  friend void swap(Model &x, Model &y) noexcept;
  virtual ~Model() { }
  Model *clone() const final;

  // Get the size of the state data in bits.
  mpz_class size_bits() const;

  bool operator==(const Node &other) const final;
  void validate() const final;

  // Get the number of (global or local) assumptions in the model.
  unsigned long assumption_count() const;

  // Update the bit offset of each variable declaration in the model.
  void reindex();
};

}
