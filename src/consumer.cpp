#include <math.h>
#include <nizer.hpp>
#include <string>
#include <vector>

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

void line_count(std::string &input, int index, int &row, int &col) {
  col = input.find_last_of("\n", index);

  if (col == std::string::npos)
    col = 0;
  else
    col = index - col;

  row = 1;
  while (true) {
    index = input.find_last_of("\n", index - 1);

    if (index == std::string::npos)
      break;

    row++;
  }
}

std::string Consumer::at(Token token) {
  int index = token.index;
  int start = source.find_last_of("\n", index);
  int end = std::min<int>(source.find("\n", index), source.size());

  int row, col;
  line_count(source, index, row, col);

  std::string code = std::string(source.begin() + start, source.begin() + end);
  code.erase(std::remove(code.begin(), code.end(), '\n'), code.end());

  return std::to_string(row) + ":" + std::to_string(col) + " '" + code + "'";
}

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
