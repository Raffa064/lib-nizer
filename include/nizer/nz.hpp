#pragma once

#include <nizer/consumer.hpp>
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <string>

namespace nz {

// TODO: Organize this file

typedef std::function<AST *(Consumer &)> ParserRule;

template <typename... Rule> AST *parseSet(Consumer &consumer, Rule... rules) {
  AST *result = nullptr;
  (void)((result = rules(consumer)) || ...);
  return result;
}

struct srcref {
  std::string source;
  int index, line, col;

  operator std::string();
};

srcref at(std::string &source, int index);

token_vector tokenize(SymbolList symbols, std::string &);

}; // namespace nz
