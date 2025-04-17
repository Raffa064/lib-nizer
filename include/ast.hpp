#pragma once

#include <any>
#include <map>
#include <vector>

class AST {
public:
  std::map<std::string, std::any> entries;

  std::string rule();

  void add(AST *node);

  AST(std::string rule);

  std::any &operator[](std::string key);
};

using ast_vector = std::vector<AST *>;

std::string ast_to_json(AST *ast);

std::string ast_to_string(AST *ast);
