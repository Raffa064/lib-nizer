#pragma once

#include <functional>
#include <nizer/ast.hpp>
#include <nizer/consumer.hpp>

namespace nz {
typedef std::function<AST *(Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;
  (void)((result = rules(consumer)) || ...);
  return result;
}

#define parser_rule(rule_name) AST *rule_name(Consumer &consumer)

} // namespace nz
