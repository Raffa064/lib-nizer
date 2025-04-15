#include <consume.h>
#include <string>
#include <vector>

int Consume::left() { return tokens.size() - index; }

bool Consume::has() { return left() > 0; }

Token Consume::get(int i) { return tokens[index + i]; }

bool Consume::current(SymMatch match) {
  if (has())
    return match(get(0));

  return false;
}

bool Consume::next(SymMatch match) {
  if (left() > 1)
    return match(get(1));

  return false;
}

bool Consume::expect(std::vector<Token *> out, std::vector<SymMatch> syms) {
  if (syms.size() > left())
    return false;

  for (int i = 0; i < syms.size(); i++) {
    Token current = tokens[index + i];

    if (!syms[i](current))
      return false;

    if (i < out.size()) {
      Token &token = *out[i];
      token = current;
    }
  }

  return true;
}

bool Consume::consume(std::vector<Token *> out, std::vector<SymMatch> syms) {
  bool as_expected = expect(out, syms);

  if (as_expected)
    index += syms.size();

  return as_expected;
}

Token Consume::consume_any() {
  Token curr = get(0);
  index++;

  return curr;
}

void Consume::skip() { index++; }

int Consume::find_next(int index, SymMatch sym) {
  for (int i = index; i < tokens.size(); i++) {
    if (sym(tokens[i]))
      return i;
  }

  return -1;
}

int Consume::find_next(SymMatch sym) { return find_next(index, sym); }

bool Consume::wrap_to(Consume &out, SymMatch end) {
  int i = index;
  int j = find_next(i, end);

  if (j < 0)
    return false;

  std::vector<Token> tks =
      std::vector<Token>(tokens.begin() + i, tokens.begin() + j);

  out = Consume(source, tks);

  index = j + 1;

  return true;
}

bool Consume::wrap(Consume &out, SymMatch start, SymMatch end) {
  if (consume({}, {start}))
    return wrap_to(out, end);

  return false;
}

std::string Consume::at() {
  if (tokens.empty())
    return "at 0";

  Token current = get(0);

  int start = current.index;
  int end = std::min<int>(start + 20, source.size());

  return "at #" + std::to_string(start) + " '" +
         std::string(source.begin() + start, source.begin() + end) + "'";
}
