#pragma once

#include <ast.h>
#include <consume.h>
#include <functional>
#include <string>
#include <sym.h>
#include <token.h>
#include <vector>

typedef std::function<AST *(Consume &)> ParserRule;

class Nizer {
public:
  SymbolList &symbols;
  ParserRule startRule;

  std::vector<Token> tokenize(std::string &);
  AST *parse(std::string);

  Nizer(SymbolList &symbols, ParserRule startRule)
      : symbols(symbols), startRule(startRule) {}
};
