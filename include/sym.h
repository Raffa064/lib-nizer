#pragma once

#include <string>
#include <token.h>
#include <vector>

enum SymFlags : int {
  REGULAR = 0,
  SKIP = 1,
  MULTLINE = 2,
};

struct SymMatch {
  int typeId;
  std::string match;
  bool excluded = false; // Exclude from output

  SymMatch(int typeId, std::string match) : typeId(typeId), match(match) {}

  bool operator()(Token);

  SymMatch operator!() {
    SymMatch copy = *this;
    copy.excluded = true;
    return copy;
  }
};

struct Symbol {
  int typeId;
  std::string pattern;
  int flags;

  Symbol() {}
  Symbol(std::string pattern, int flags) : pattern(pattern), flags(flags) {};

  operator SymMatch();

  SymMatch operator()(std::string);
  bool operator()(Token);
};

class SymbolList : public std::vector<Symbol> {
  int counter = 0;

public:
  Symbol operator()(std::string pattern, int flags) {
    Symbol sym = Symbol(pattern, flags);
    sym.typeId = counter++;

    push_back(sym);

    return sym;
  }

  Symbol operator()(Symbol sym) { return (*this)(sym.pattern, sym.flags); }
};

namespace NizerSymbols {
const Symbol WS = Symbol("[ \n\r\t]+", MULTLINE | SKIP);
const Symbol SHARP_COMMENT = Symbol("#[^\n\r]*", SKIP);
const Symbol INLINE_COMMENT = Symbol("//.*", SKIP);
const Symbol MULTLINE_COMMENT = Symbol("/\\*(.*)\\*/", SKIP | MULTLINE);
const Symbol BASIC_ID = Symbol("[a-zA-Z$_][a-zA-Z$_0-9]*", REGULAR);
} // namespace NizerSymbols
