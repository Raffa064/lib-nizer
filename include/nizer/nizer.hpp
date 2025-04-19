#pragma once

#include <nizer/parser.hpp>
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <nizer/visitor.hpp>
#include <string>
#include <vector>

class Nizer {
public:
  SymbolList &symbols;
  ParserRule startRule;
  std::vector<Visitor> visitors;
  bool logParser;

  void add_visitor(Visitor);
  token_vector tokenize(std::string &);
  AST *wrap(std::string ruleName, ParserRule rule, Consumer &consumer);
  AST *parse(std::string);

  Nizer(SymbolList &symbols, ParserRule startRule)
      : symbols(symbols), startRule(startRule) {}
};
