#include <ast.hpp>
#include <consumer.hpp>
#include <functional>

typedef std::function<AST *(Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;

  (void)((result = rules(consumer)) || ...);

  return result;
}
