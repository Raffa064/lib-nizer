#include "nizer/nz.hpp"
#include <math.h>
#include <nizer/consumer.hpp>
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <string>
#include <vector>

namespace nz {

int Consumer::left() { return tokens.size() - index; }

bool Consumer::has() { return left() > 0; }

Token Consumer::get(int i) { return tokens[index + i]; }

bool Consumer::expect(std::vector<Token *> out, std::vector<SymMatch> syms) {
  if (syms.size() > left())
    return false;

  int j = 0;
  for (int i = 0; i < syms.size(); i++) {
    Token current = tokens[index + i];

    if (!syms[i](current))
      return false;

    if (!syms[i].excluded) {
      if (j < out.size()) {
        Token &token = *out[j];
        token = current;
      }

      j++;
    }
  }

  return true;
}

bool Consumer::expect(std::vector<SymMatch> syms) { return expect({}, syms); }

bool Consumer::current(SymMatch match) {
  if (has())
    return match(get(0));

  return false;
}

bool Consumer::next(SymMatch match) {
  if (left() > 1)
    return match(get(1));

  return false;
}

bool Consumer::consume(std::vector<Token *> out, std::vector<SymMatch> syms) {
  bool as_expected = expect(out, syms);

  if (as_expected)
    index += syms.size();

  return as_expected;
}

bool Consumer::consume(std::vector<SymMatch> syms) { return consume({}, syms); }

bool Consumer::consume_sym(SymMatch sym) { return consume({}, {sym}); }

Token Consumer::consume_any() {
  Token curr = get(0);
  index++;

  return curr;
}

void Consumer::skip() { index++; }

int Consumer::find_next(int index, SymMatch sym) {
  for (int i = index; i < tokens.size(); i++) {
    if (sym(tokens[i]))
      return i;
  }

  return -1;
}

int Consumer::find_next(SymMatch sym) { return find_next(index, sym); }

bool Consumer::wrap_to(Consumer &out, SymMatch end) {
  int i = index;
  int j = find_next(i, end);

  if (j < 0)
    return false;

  std::vector<Token> tks =
      std::vector<Token>(tokens.begin() + i, tokens.begin() + j);

  out = Consumer(source, tks);

  index = j + 1;

  return true;
}

bool Consumer::wrap(Consumer &out, SymMatch start, SymMatch end) {
  if (consume({}, {start}))
    return wrap_to(out, end);

  return false;
}

std::string Consumer::at(Token token) { return nz::at(source, token.index); }

std::string Consumer::at() {
  if (!has())
    return "EOF"; // reached enf of file (or end of line)

  Token currentToken = get(0);
  return at(currentToken);
}

void Consumer::debug() {
  std::vector<Token> tks(tokens.begin() + index, tokens.end());
  print_tokens(tks);
}

AST *Consumer::make_node(std::string rule, AST::entry_map entries) {
  int index = has() ? get(0).index : 0;
  AST &node = *new AST(source, index, rule, entries);

  return &node;
}

} // namespace nz
