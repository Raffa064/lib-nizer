#include <any>
#include <ast.hpp>
#include <sstream>
#include <string>

std::string AST::rule() { return std::any_cast<std::string>(entries["rule"]); }

AST::AST(std::string rule) { entries["rule"] = rule; }

std::any &AST::operator[](std::string key) { return entries[key]; }

std::string quote(std::string str) { return "\"" + str + "\""; }

std::string any_to_json(std::any value) {
  if (value.type() == typeid(std::vector<AST *> *)) {
    auto nodes = *std::any_cast<std::vector<AST *> *>(value);

    std::stringstream buffer;
    buffer << "[";

    bool first = true;
    for (AST *node : nodes) {
      if (!first)
        buffer << ",";

      buffer << ast_to_json(node);
      first = false;
    }

    buffer << "]";

    return buffer.str();
  }

  if (value.type() == typeid(std::string))
    return quote(any_cast<std::string>(value));

  if (value.type() == typeid(const char *))
    return quote(std::string(any_cast<const char *>(value)));

#define SIMPLE_CAST(_type)                                                     \
  if (value.type() == typeid(_type))                                           \
    return std::to_string(any_cast<_type>(value));

  SIMPLE_CAST(int)
  SIMPLE_CAST(float)
  SIMPLE_CAST(double)
  SIMPLE_CAST(bool)

  if (value.type() == typeid(AST *))
    return ast_to_json(any_cast<AST *>(value));

  return "<unprintable-type>";
}

std::string ast_to_json(AST *ast, std::stringstream &buffer) {
  buffer << "{";

  bool first = true;
  for (auto &[key, value] : ast->entries) {
    if (!first)
      buffer << ",";

    std::string str_value = any_to_json(value);
    buffer << quote(key) << ": " << str_value;
    first = false;
  }

  buffer << "}";

  return buffer.str();
}

std::string ast_to_json(AST *ast) {
  std::stringstream buffer;
  return ast_to_json(ast, buffer);
}

/*
std::string ast_to_string(AST *ast, std::stringstream &buffer) {
  buffer << "(" << ast->rule();

  aut
  for (AST *node : *nodes)
    buffer << " " << ast_to_string(node);

  buffer << ")";

  return buffer.str();
}

std::string ast_to_string(AST *ast) {
  std::stringstream buffer;
  return ast_to_string(ast, buffer);
}
*/
