#include <functional>
#include <nizer.hpp>

typedef std::function<AST *(Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;

  (void)((result = rules(consumer)) || ...);

  return result;
}

#define EXPAND(x) x
#define JOIN(a, b, c) a##b##c
#define UNAME(a, b) EXPAND(JOIN(a, _, b))

#define _wrap_rule(rule) rule(consumer)

#define _parser_rule(rule_name, id)                                            \
  AST *UNAME(rule_name, id)(Consumer & consumer);                              \
  AST *rule_name(Consumer &consumer) {                                         \
    return _wrap_rule(UNAME(rule_name, id));                                   \
  }                                                                            \
  /*unwrapped*/ AST *UNAME(rule_name, id)(Consumer & consumer)

#define parser_rule(name) _parser_rule(name, __COUNTER__)
