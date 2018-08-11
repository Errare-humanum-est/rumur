#pragma once

#include "location.hh"
#include <rumur/Decl.h>
#include <rumur/Node.h>
#include <rumur/TypeExpr.h>
#include <string>
#include <vector>

namespace rumur {

struct Parameter : public Node {

  VarDecl *decl;
  bool by_reference;

  Parameter() = delete;
  Parameter(VarDecl *decl_, bool by_reference_, const location &loc_);
  Parameter(const Parameter &other);
  Parameter &operator=(Parameter other);
  friend void swap(Parameter &x, Parameter &y) noexcept;
  virtual ~Parameter();
  Parameter *clone() const final;
  bool operator==(const Node &other) const final;
};

struct Function : public Node {

  std::string name;
  std::vector<Parameter*> parameters;
  std::vector<Decl*> decls;
  TypeExpr *return_type;

  Function() = delete;
  Function(const std::string &name_, std::vector<Parameter*> &&parameters_,
    std::vector<Decl*> &&decls_, TypeExpr *return_type_, const location &loc_);
  Function(const Function &other);
  Function &operator=(Function other);
  friend void swap(Function &x, Function &y) noexcept;
  virtual ~Function();
  Function *clone() const final;
  bool operator==(const Node &other) const final;
};

}
