#include <ast.h>
#include <iostream>

void print_ast(AST *node, bool is_root) {
  std::cout << "( " << node->to_string() << " ";

  for (AST *n : node->nodes())
    print_ast(n, false);

  std::cout << ")";

  if (is_root)
    std::cout << std::endl;
}

void print_ast(AST *root) { print_ast(root, true); }
