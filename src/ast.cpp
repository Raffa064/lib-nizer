#include <any>
#include <nizer/ast.hpp>
#include <sstream>
#include <string>

std::string AST::rule() { return std::any_cast<std::string>(entries["rule"]); }

AST::AST(std::string rule, entry_map _entries) {
  entries = _entries;
  entries["rule"] = rule;
}

AST::~AST() {
  for (auto &[key, value] : entries) {
    if (value.type() == typeid(AST *))
      delete std::any_cast<AST *>(value);

    if (value.type() == typeid(ast_vector)) {
      ast_vector nodes = std::any_cast<ast_vector>(value);

      for (auto n : nodes)
        delete n;
    }
  }
}

std::any &AST::operator[](std::string key) { return entries[key]; }

std::string quote(std::string str) { return "\"" + str + "\""; }

std::string any_to_json(std::any value) {
  if (value.type() == typeid(ast_vector)) {
    auto nodes = std::any_cast<ast_vector>(value);

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

void ast_to_json(AST *ast, std::stringstream &buffer) {
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
}

std::string ast_to_json(AST *ast) {
  std::stringstream buffer;
  ast_to_json(ast, buffer);

  return buffer.str();
}

void ast_to_string(AST *ast, std::stringstream &buffer,
                   std::string label = "") {

  if (!label.empty())
    label += ":";

  buffer << " (" << label << ast->rule();

  for (auto &[key, value] : ast->entries) {
    if (value.type() == typeid(ast_vector)) {
      ast_vector nodes = any_cast<ast_vector>(value);

      buffer << " [" << key;
      for (auto node : nodes)
        ast_to_string(node, buffer);

      buffer << "]";
    }

    if (value.type() == typeid(AST *)) {
      AST *node = std::any_cast<AST *>(value);
      ast_to_string(node, buffer, key);
    }
  }

  buffer << ")";
}

std::string ast_to_string(AST *ast) {
  std::stringstream buffer;
  ast_to_string(ast, buffer);

  return buffer.str();
}
