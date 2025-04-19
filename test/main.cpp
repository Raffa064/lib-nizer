#include <iostream>
#include <nizer.hpp>
#include <ostream>
#include <regex>
#include <string>

SymbolList symbols;

Symbol NUM = symbols("[0-9]+", REGULAR);      // Números inteiros
Symbol OP = symbols("[\\+\\-\\*/]", REGULAR); // Operadores + - * /
Symbol WS = symbols(NizerSymbols::WS);        // Espaços em branco ignorados

parser_rule(parseFactor) {
  Token num;
  if (consumer.consume({&num}, {NUM})) {
    AST &node = *new AST("factor", {{"value", std::stoi(num.value)}});
    return &node;
  }

  throw nizer_error("Invalid token at {}", {consumer.at()});
}

// Multiplicação e divisão
parser_rule(parseTerm) {
  AST *left = parseFactor(nizer, consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\*/]")})) {
    AST *right = parseFactor(nizer, consumer);

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
  AST *left = parseTerm(nizer, consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\+\\-]")})) {
    AST *right = parseTerm(nizer, consumer);

    AST &node = *new AST("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  return left;
}

visitor(optimizer) {
  visit("op") {
    AST &left = *optimizer(node->get<AST *>("left"));
    AST &right = *optimizer(node->get<AST *>("right"));

    if (left.rule() != "factor" || right.rule() != "factor")
      return node;

    int lValue = left.get<int>("value");
    int rValue = right.get<int>("value");

    int result;
    std::string op = node->get<std::string>("op");

    if (op == "+")
      result = lValue + rValue;
    else if (op == "-")
      result = lValue - rValue;
    else if (op == "*")
      result = lValue * rValue;
    else
      result = lValue / rValue;

    node->reset("factor", {{"value", result}});
  }

  return node;
}

int main() {
  Nizer nizer(symbols, parseMath);
  nizer.add_visitor(optimizer);

  std::string exp = "10 + 2 * 3 * 3 - 4 * 2";
  AST *tree = nizer.parse(exp);

  std::cout << exp << std::endl;
  std::cout << "Result: " << tree->get<int>("value") << std::endl;

  delete tree;

  return 0;
}
