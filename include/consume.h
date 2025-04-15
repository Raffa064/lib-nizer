#pragma once

#include <string>
#include <sym.h>
#include <token.h>
#include <vector>

class Consume {
  std::string source;
  std::vector<Token> tokens;
  int index;

public:
  int left();
  bool has();

  Token get(int);

  bool current(SymMatch);
  bool next(SymMatch);

  bool expect(std::vector<Token *>, std::vector<SymMatch>);
  bool consume(std::vector<Token *>, std::vector<SymMatch>);

  Token consume_any();

  void skip();

  int find_next(int, SymMatch);
  int find_next(SymMatch);

  bool wrap_to(Consume &, SymMatch);

  bool wrap(Consume &, SymMatch, SymMatch);

  std::string at();

  Consume(std::string source, std::vector<Token> tokens)
      : source(source), tokens(tokens), index(0) {};

  Consume() : Consume("", {}) {}
};
