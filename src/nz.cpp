#include "nizer/err.hpp"
#include <nizer/ast.hpp>
#include <nizer/consumer.hpp>
#include <nizer/nz.hpp>
#include <nizer/token.hpp>
#include <nizer/visitor.hpp>
#include <regex>

namespace nz {

token_vector tokenize(SymbolList symbols, std::string &source) {
  token_vector tokens;

  int i = 0;
  while (i < source.length()) {
    bool matched = false;

    for (Symbol sym : symbols) {
      std::regex_constants::syntax_option_type flags = std::regex::ECMAScript;

      if ((sym.flags & MULTLINE) != 0)
        flags |= std::regex::multiline;

      std::regex regex("^" + sym.pattern, flags);
      std::smatch match;

      if (std::regex_search(source.cbegin() + i, source.cend(), match, regex)) {
        std::string value = match.str();

        if ((sym.flags & SKIP) == 0)
          tokens.push_back({.typeId = sym.typeId, .value = value, .index = i});

        matched = true;
        i += value.length();
        break;
      }
    }

    if (!matched) {
      throw nz::error("Can't apply match at {}", {nz::at(source, i)});
    }
  }

  return tokens;
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

std::string at(std::string source, int index) {
  int start = source.find_last_of("\n", index);
  int end = source.find("\n", index);

  if (start == std::string::npos)
    start = 0;
  if (end == std::string::npos)
    end = source.size();

  int row, col;
  line_count(source, index, row, col);

  std::string code = std::string(source.begin() + start, source.begin() + end);
  code.erase(std::remove(code.begin(), code.end(), '\n'), code.end());

  return std::to_string(row) + ":" + std::to_string(col) + " '" + code + "'";
}

} // namespace nz
