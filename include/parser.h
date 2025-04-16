#include <ast.h>
#include <consumer.h>
#include <functional>

typedef std::function<AST *(Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;

  (void)((result = rules(consumer)) || ...);

  return result;
}
