#include "nizer/ast.hpp"
#include "nizer/err.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <use-nizer.hpp>

SymbolList symbols;

Symbol NUM = symbols("[0-9]+", nz::REGULAR);      // Números inteiros
Symbol OP = symbols("[\\+\\-\\*/]", nz::REGULAR); // Operadores + - * /
Symbol WS = symbols(nz::sym::WS);                 // Espaços em branco ignorados

parser_rule(parseFactor) {
  Token num;
  if (consumer.consume({&num}, {NUM})) {
    AST &node = *new AST("factor", {{"value", std::stoi(num.value)}});
    return &node;
  }

  throw nz::error("Invalid token at {}", {consumer.at()});
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

int visitor(evaluator) {
  visit("op") {
    int left = evaluator(node->get<AST *>("left"));
    int right = evaluator(node->get<AST *>("right"));

    int result;
    std::string op = node->get<std::string>("op");

    if (op == "+")
      result = left + right;
    else if (op == "-")
      result = left - right;
    else if (op == "*")
      result = left * right;
    else
      result = left / right;

    return result;
  }

  visit("factor") { return node->get<int>("value"); }

  throw nz::error("Invalid node: {}", {nz::ast_to_string(node)});
}

int main() {
  try {
    std::string exp = "10 * 2 / 3 - 4";

    token_vector tokens = nz::tokenize(symbols, exp);
    Consumer consumer(exp, tokens);
    AST *tree = parseMath(consumer);
    int value = evaluator(tree);

    std::cout << nz::ast_to_string(tree) << std::endl;
    std::cout << exp << " = " << value << std::endl;

    delete tree;

    return 0;
  } catch (nz::error err) {
    std::cout << "Error:" << err.msg << std::endl;
    return 1;
  }
}
