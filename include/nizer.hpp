#pragma once

#include <nizer/ast.hpp>
#include <nizer/token.hpp>

#include <nizer/sym.hpp>

#include <nizer/consumer.hpp>
#include <nizer/err.hpp>
#include <nizer/parser.hpp>

#include <string>

class Nizer {
public:
  SymbolList &symbols;
  ParserRule startRule;

  token_vector tokenize(std::string &);
  AST *parse(std::string);

  Nizer(SymbolList &symbols, ParserRule startRule)
      : symbols(symbols), startRule(startRule) {}
};
