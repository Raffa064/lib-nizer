#include <nizer/ast.hpp>
#include <regex>

namespace nz {

#define visitor(name) name(AST *node)

#define EXPAND(x) x
#define JOIN(a, b, c) a##b##c
#define UNAME(a, b) EXPAND(JOIN(a, _, b))

#define _visit(pattern, id)                                                    \
  std::regex UNAME(pat, id)(pattern);                                          \
  if (std::regex_match(node->rule(), UNAME(pat, id)))

#define visit(pattern) _visit(pattern, __COUNTER__)

} // namespace nz
