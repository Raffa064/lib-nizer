#pragma once

#include <functional>
#include <nizer/ast.hpp>
#include <nizer/consumer.hpp>
#include <nizer/macros.hpp>

class Nizer;
typedef std::function<AST *(Nizer &, Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;

  (void)((result = rules(consumer)) || ...);

  return result;
}

#define _parser_rule(rule_name, id)                                            \
  AST *UNAME(rule_name, id)(Nizer & nizer, Consumer & consumer);               \
  AST *rule_name(Nizer &nizer, Consumer &consumer) {                           \
    return nizer.wrap(#rule_name, UNAME(rule_name, id), consumer);             \
  }                                                                            \
  AST *UNAME(rule_name, id)(Nizer & nizer, Consumer & consumer)

#define parser_rule(name) _parser_rule(name, __COUNTER__)
