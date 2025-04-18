#pragma once

#include <any>
#include <map>
#include <vector>

class AST {
public:
  using entry_map = std::map<std::string, std::any>;

  entry_map entries;

  std::string rule();

  AST(std::string rule, entry_map entries = {});

  std::any &operator[](std::string key);

  ~AST();
};

using ast_vector = std::vector<AST *>;

std::string ast_to_json(AST *ast);

std::string ast_to_string(AST *ast);
