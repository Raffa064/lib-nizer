#pragma once

#include <ast.h>
#include <consumer.h>
#include <parser.h>
#include <string>
#include <sym.h>
#include <token.h>
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
