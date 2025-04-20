#pragma once

#include <any>
#include <map>
#include <vector>

namespace nz {

class AST {
public:
  using entry_map = std::map<std::string, std::any>;

  entry_map entries;

  std::string rule();

  template <typename T> T get(std::string key) {
    return std::any_cast<T>(entries[key]);
  }

  void reset(std::string rule, entry_map entries = {});

  void destroyChildren();

  AST(std::string rule, entry_map entries = {}) { reset(rule, entries); }

  std::any &operator[](std::string key);

  ~AST();
};

using ast_vector = std::vector<AST *>;

std::string ast_to_json(AST *ast);

std::string ast_to_string(AST *ast);

} // namespace nz
