#pragma once

#include "nizer/ast.hpp"
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <string>
#include <vector>

namespace nz {

class Consumer {
  std::string source;
  token_vector tokens;
  int index;

public:
  // Returns how many tokens are left to consume
  int left();

  // Returns true if there are tokens left to consume
  bool has();

  // Returns the token at a given offset relative to the current index
  Token get(int);

  /*
   * Checks if the upcoming tokens match a sequence of symbols.
   * If true, stores pointers to the matching tokens in the first parameter.
   */
  bool expect(std::vector<Token *>, std::vector<SymMatch>);

  // Checks if the upcoming tokens match a sequence of symbols
  bool expect(std::vector<SymMatch>);

  // Checks if the current token matches a given symbol
  bool current(SymMatch);

  // Checks if the next token matches a given symbol
  bool next(SymMatch);

  /*
   * Same as expect(), but also advances the current index if the match
   * succeeds. Matching tokens are stored in the first parameter.
   */
  bool consume(std::vector<Token *>, std::vector<SymMatch>);

  // Same as consume() but discards token references
  bool consume(std::vector<SymMatch>);

  // Same as consume() but for a single symbol match
  bool consume_sym(SymMatch);

  // Consumes and returns the current token
  Token consume_any();

  // Skips the current token
  void skip();

  // Returns relative index to the next occurrence of a symbol, starting from a
  // given position
  int find_next(int, SymMatch);

  // Returns relative index to the next occurrence of a symbol, starting from
  // the current index
  int find_next(SymMatch);

  /*
   * Creates a new Consumer starting at the current index
   * and ending at the next token matching the given symbol.
   */
  bool wrap_to(Consumer &, SymMatch);

  /*
   * Creates a new Consumer from the next occurrence of the first symbol
   * to the next occurrence of the second symbol.
   */
  bool wrap(Consumer &, SymMatch, SymMatch);

  // Return a string with location of the given token in the source
  std::string at(Token);

  // Returns a string with the location of the current token in the source
  std::string at();

  // Display current tokens
  void debug();

  // Creates a new node, adding metadata into it
  AST *make_node(std::string name, AST::entry_map entries = {});

  Consumer(std::string source, token_vector tokens)
      : source(source), tokens(tokens), index(0) {};

  Consumer() : Consumer("", {}) {}
};

} // namespace nz
