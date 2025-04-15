#pragma once

#include <vector>

class AST {
public:
  virtual std::vector<AST *> nodes() = 0;

  virtual std::string to_string() = 0;
};

class ASTLeaf : public AST {
  std::vector<AST *> nodes() override { return {}; }
};

void print_ast(AST *);
