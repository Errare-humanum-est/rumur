#include <cstddef>
#include "CLikeGenerator.h"
#include "generate_h.h"
#include <iostream>
#include "resources.h"
#include <rumur/rumur.h>

using namespace rumur;

namespace {

class HGenerator : public CLikeGenerator {

 public:
  HGenerator(std::ostream &out_, bool pack_): CLikeGenerator(out_, pack_) { }

  void visit_constdecl(const ConstDecl &n) final {
    *this << indentation() << "extern const ";
    if (n.type == nullptr) {
      *this << "int64_t";
    } else {
      *this << *n.type;
    }
    *this << " " << n.name << ";\n";
  }

  void visit_function(const Function &n) final {
    *this << indentation();
    if (n.return_type == nullptr) {
      *this << "void";
    } else {
      *this << *n.return_type;
    }
    *this << " " << n.name << "(";
    bool first = true;
    for (const Ptr<VarDecl> &p : n.parameters) {
      if (!first) {
        *this << ", ";
      }
      *this << *p->type << " ";
      // if this is a var parameter, it needs to be a pointer
      if (!p->readonly) {
        *this << "*" << p->name << "_";
      } else {
        *this << p->name;
      }
      first = false;
    }
    *this << ");\n";
  }

  void visit_propertyrule(const PropertyRule &n) final {

    // function prototype
    *this << indentation() << "bool " << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ");\n";
  }

  void visit_simplerule(const SimpleRule &n) final {
    *this << indentation() << "bool guard_" << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ");\n\n";

    *this << indentation() << "void rule_" << n.name << "(";

    // parameters
    first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ")\n";
  }

  void visit_startstate(const StartState &n) final {
    *this << indentation() << "void startstate_" << n.name << "(";

    // parameters
    bool first = true;
    for (const Quantifier &q : n.quantifiers) {
      if (!first) {
        *this << ", ";
      }
      if (auto t = dynamic_cast<const TypeExprID*>(q.type.get())) {
        *this << t->name;
      } else {
        *this << "int64_t";
      }
      *this << " " << q.name;
      first = false;
    }

    *this << ");\n";
  }

  void visit_vardecl(const VarDecl &n) final {
    *this << indentation() << "extern " << *n.type << " " << n.name << ";\n";
  }
};

}

void generate_h(const Node &n, bool pack, std::ostream &out) {

  // write the static prefix to the beginning of the source file
  for (size_t i = 0; i < resources_h_prefix_h_len; i++)
    out << (char)resources_h_prefix_h[i];

  HGenerator gen(out, pack);
  gen.dispatch(n);

  // close the `extern "C"` block opened in ../resources/h_prefix.h
  out
    << "\n"
    << "#ifdef __cplusplus\n"
    << "}\n"
    << "#endif\n";
}
