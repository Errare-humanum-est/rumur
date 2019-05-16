#include <cstddef>
#include "generate.h"
#include <iostream>
#include <memory>
#include <rumur/rumur.h>
#include "utils.h"
#include <vector>

void generate_function(std::ostream &out, const rumur::Function &f,
    const std::vector<rumur::Ptr<rumur::Decl>> &decls) {

  /* Functions returning a simple type return a value, as expected. Functions
   * returning a complex type return a handle that is actually the same as their
   * second parameter (see below).
   */
  out << "static ";
  if (f.return_type == nullptr) {
    /* We need to give void-returning functions a dummy boolean return type to
     * align with the type signature for rules. More specifically a return
     * can appear either within a rule or within a function/procedure. Within a
     * rule, it needs to return true to indicate to the caller no errors were
     * encountered during the rule. To allow a return statement to be emitted
     * uniformly without having to first check whether its within a rule or a
     * function/procedure we make the latter return a (ignored) boolean as well.
     */
    out << "bool";
  } else if (f.return_type->is_simple()) {
    out << "value_t";
  } else {
    out << "struct handle";
  }
  out << " ru_" << f.name << "(const char *rule_name __attribute__((unused)), "
    << "struct state *NONNULL s __attribute__((unused))";

  // If required, generate the return (out) parameter.
  if (f.return_type != nullptr && !f.return_type->is_simple())
    out << ", struct handle ret";

  for (const rumur::Ptr<rumur::VarDecl> &p : f.parameters)
    out << ", struct handle ru_" << p->name;

  out << ") {\n";

  /* Output the state variable handles so we can reference them within
   * this start state.
   */
  for (const rumur::Ptr<rumur::Decl> &d : decls) {
    if (isa<rumur::VarDecl>(d)) {

      /* Exciting kludge: we need to suppress the definition of state variables
       * that are shadowed by function parameters. Yes, real world models seem
       * to do this.
       */
      bool shadowed = false;
      for (const rumur::Ptr<rumur::VarDecl> &p : f.parameters) {
        if (p->name == d->name) {
          shadowed = true;
          break;
        }
      }
      if (shadowed)
        continue;

      out << "  ";
      generate_decl(out, *d);
      out << ";\n";
    }
  }

  /* Open a scope to support local declarations can shadow the state
   * variables.
   */
  out << "  {\n";

  // Output this function's local decls
  for (const rumur::Ptr<rumur::Decl> &d : f.decls) {
    if (isa<rumur::ConstDecl>(d) || isa<rumur::VarDecl>(d)) {
      out << "  ";
      generate_decl(out, *d);
      out << ";\n";
    }
  }

  // Allocate memory for any complex-returning functions we call
  generate_allocations(out, f.body);

  // Generate the body of the function
  for (auto &s : f.body) {
    out << "  ";
    generate_stmt(out, *s);
    out << ";\n";
  }

  /* Emit a guard against the user leaving a control flow path through their
   * function that doesn't return.
   */
  if (f.return_type != nullptr)
    out << "  error(s, \"The end of function %s reached without returning "
      << "values.\", \"" << f.name << "\");\n";

  if (f.return_type == nullptr)
    out << "  return true; /* ignored by caller */\n";

  // Close the scope we created.
  out << "  }\n";

  out << "}";
}
