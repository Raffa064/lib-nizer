#include <functional>
#include <nizer/ast.hpp>
#include <nizer/macros.hpp>

using Visitor = std::function<AST *(AST *)>;

#define visitor(name) AST *name(AST *node)

#define _visit(pattern, id)                                                    \
  std::regex pat##id(pattern);                                                 \
  if (std::regex_match(node->rule(), pat##id))

#define visit(pattern) _visit(pattern, __COUNTER__)
