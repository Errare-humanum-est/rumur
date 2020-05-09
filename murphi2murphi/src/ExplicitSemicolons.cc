#include <cstddef>
#include <cassert>
#include <ctype.h>
#include "ExplicitSemicolons.h"
#include <rumur/rumur.h>

using namespace rumur;

ExplicitSemicolons::ExplicitSemicolons(Stage &next_):
  IntermediateStage(next_) { }

void ExplicitSemicolons::process(const Token &t) {

  // if we are not waiting on a semi-colons, we can simply output this character
  if (!pending_semi) {
    assert(pending.empty());
    next.process(t);
    return;
  }

  switch (t.type) {

    case Token::CHAR:
      // if this is white space, keep accruing pending characters
      if (t.character.size() == 1 && isspace(t.character.c_str()[0])) {
        pending.push_back(t);
        return;
      }
      break;

    // if this was a shift message to another Stage, accrue it
    case Token::SUBJ:
      pending.push_back(t);
      return;
  }

  // if we reached here, we know one way or another we are done accruing
  pending_semi = false;

  // the semi-colon was either explicit already if this character itself is a
  // semi-colon, or it was omitted otherwise
  if (t.type == Token::CHAR && t.character != ";")
    next << ";";

  // flush the accrued white space and shift messages
  flush();

  next.process(t);
}

// each of these need to simply passthrough to the next stage in the pipeline
// and note that we then have a pending semi-colon
void ExplicitSemicolons::visit_aliasrule(const AliasRule &n) {
  next.visit_aliasrule(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_constdecl(const ConstDecl &n) {
  next.visit_constdecl(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_function(const Function &n) {
  next.visit_function(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_propertyrule(const PropertyRule &n) {
  next.visit_propertyrule(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_ruleset(const Ruleset &n) {
  next.visit_ruleset(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_simplerule(const SimpleRule &n) {
  next.dispatch(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_startstate(const StartState &n) {
  next.visit_startstate(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_typedecl(const TypeDecl &n) {
  next.visit_typedecl(n);
  pending_semi = true;
}
void ExplicitSemicolons::visit_vardecl(const VarDecl &n) {
  next.visit_vardecl(n);
  pending_semi = true;
}

void ExplicitSemicolons::finalise() {

  // if we have a pending semi-colon, we know we are never going to see one now
  if (pending_semi)
    next << ";";
  pending_semi = false;

  flush();
}

void ExplicitSemicolons::flush() {
  for (const Token &t : pending)
    next.process(t);
  pending.clear();
}
