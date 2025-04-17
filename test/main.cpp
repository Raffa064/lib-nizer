#include "ast.hpp"
#include "consumer.hpp"
#include "err.hpp"
#include "nizer.hpp"
#include "sym.hpp"
#include <iostream>
#include <ostream>
#include <string>

SymbolList symbols;

Symbol NUM = symbols("[0-9]+", REGULAR);      // Números inteiros
Symbol OP = symbols("[\\+\\-\\*/]", REGULAR); // Operadores + - * /
Symbol WS = symbols(NizerSymbols::WS);        // Espaços em branco ignorados

AST *parseFactor(Consumer &consumer);

AST(*parseFactor(Consumer &consumer)) {
  Token num;
  if (consumer.consume({&num}, {NUM})) {
    AST &node = *new AST("factor");
    node["value"] = std::stoi(num.value);
    return &node;
  }

  throw nizer_error("Invalid token at {}", {consumer.at()});
}

// Multiplicação e divisão
AST *parseTerm(Consumer &consumer) {
  AST *left = parseFactor(consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\*/]")})) {
    AST *right = parseFactor(consumer);

    AST &node = *new AST("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  return left;
}

// Soma e subtração
AST *parseMath(Consumer &consumer) {
  AST *left = parseTerm(consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\+\\-]")})) {
    AST *right = parseTerm(consumer);

    AST &node = *new AST("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  return left;
}

int main() {
  Nizer nizer(symbols, parseMath);

  AST *tree = nizer.parse("10 + 2 * 3 - 4");

  std::cout << ast_to_string(tree) << std::endl;

  return 0;
}
