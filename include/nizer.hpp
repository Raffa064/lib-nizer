#pragma once

#include <ast.hpp>
#include <consumer.hpp>
#include <parser.hpp>
#include <string>
#include <sym.hpp>
#include <token.hpp>
#include <vector>

class Nizer {
public:
  SymbolList &symbols;
  ParserRule startRule;

  std::vector<Token> tokenize(std::string &);
  AST *parse(std::string);

  Nizer(SymbolList &symbols, ParserRule startRule)
      : symbols(symbols), startRule(startRule) {}
};
