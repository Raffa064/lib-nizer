#include <nizer.h>
#include <regex>
#include <stdexcept>
#include <vector>

std::vector<Token> Nizer::tokenize(std::string &source) {
  std::vector<Token> tokens;

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
      throw std::runtime_error(
          "Can't apply match at #" + std::to_string(i) + " : '" +
          source.substr(i, std::min<int>(source.length(), 20)) + "'");
    }
  }

  return tokens;
}

AST *Nizer::parse(std::string source) {
  std::vector<Token> tokens = tokenize(source);

  Consumer consume(source, tokens);

  return startRule(consume);
}
