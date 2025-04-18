#include <iostream>
#include <nizer.hpp>
#include <ostream>
#include <string>

SymbolList symbols;

Symbol NUM = symbols("[0-9]+", REGULAR);      // Números inteiros
Symbol OP = symbols("[\\+\\-\\*/]", REGULAR); // Operadores + - * /
Symbol WS = symbols(NizerSymbols::WS);        // Espaços em branco ignorados

parser_rule(parseFactor) {
  Token num;
  if (consumer.consume({&num}, {NUM})) {
    AST &node = *new AST("factor");
    node["value"] = std::stoi(num.value);
    return &node;
  }

  throw nizer_error("Invalid token at {}", {consumer.at()});
}

// Multiplicação e divisão
parser_rule(parseTerm) {
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
parser_rule(parseMath) {
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

// transformer(optmizer) { return && }

int main() {
  Nizer nizer(symbols, parseMath);
  // nizer.add_transformer(optimizer);

  AST *tree = nizer.parse("10 + 2 * 3 - 4");

  std::cout << ast_to_string(tree) << std::endl;

  return 0;
}
