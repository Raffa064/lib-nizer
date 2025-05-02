#pragma once

#include <any>
#include <map>
#include <vector>

namespace nz {

struct srcref;

class Consumer;

class AST {
public:
  std::string &source;
  int index;

  using entry_map = std::map<std::string, std::any>;
  entry_map entries;

  std::string rule();

  template <typename T> T get(std::string key) {
    std::any any = entries[key];

    // Workaround for accept const char* as std::string
    if (typeid(T) != any.type() && typeid(T) == typeid(std::string)) {
      auto c_str = std::any_cast<const char *>(any);
      any = std::string(c_str);
      return std::any_cast<T>(any);
    }

    return std::any_cast<T>(any);
  }

  srcref at();

  void reset(std::string rule, entry_map entries = {});

  void destroyChildren();

  std::any &operator[](std::string key);

  bool operator==(std::string rule);

  ~AST();

private:
  AST(std::string &source, int index, std::string rule, entry_map entries)
      : source(source), index(index) {
    reset(rule, entries);
  }

  friend Consumer;
};

using ast_vector = std::vector<AST *>;

std::string ast_to_json(AST *ast);

std::string ast_to_string(AST *ast);

} // namespace nz
