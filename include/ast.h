#pragma once

#include <any>
#include <map>

class AST {
public:
  std::map<std::string, std::any> entries;

  std::string rule();

  std::vector<AST *> *nodes();

  void add(AST *node);

  AST(std::string rule, std::vector<AST *> nodes = {});

  std::any &operator[](std::string key);
};

std::string ast_to_json(AST *ast);

std::string ast_to_string(AST *ast);
