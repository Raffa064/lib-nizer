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

AST *parseFactor(Consumer &consumer) {
  Token num;
  if (consumer.consume({&num}, {NUM})) {
    AST &node =
        *consumer.make_node("factor", {{"value", std::stoi(num.value)}});
    return &node;
  }

  throw nz::error("Syntax Error", "Invalid token", consumer.at());
}

// Multiplicação e divisão
AST *parseTerm(Consumer &consumer) {
  AST *left = parseFactor(consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\*/]")})) {
    AST *right = parseFactor(consumer);

    AST &node = *consumer.make_node("op");
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

    AST &node = *consumer.make_node("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  if (consumer.has())
    throw nz::error("Dangling tokens", consumer.at());

  return left;
}

int evaluator(AST *node) {
  if (*node == "op") {
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

  if (*node == "factor")
    return node->get<int>("value");

  throw nz::error("Invalid node:" + nz::ast_to_string(node), node->at());
}

int calculateExp(std::string exp) {
  token_vector tokens = nz::tokenize(symbols, exp);
  Consumer consumer(exp, tokens);
  AST *tree = parseMath(consumer);
  int value = evaluator(tree);
  delete tree;

  return value;
}

int main() {
  while (true) {
    try {
      std::cout << ">> ";

      std::string exp;
      std::getline(std::cin, exp);

      int result = calculateExp(exp);
      std::cout << " = " << result << std::endl;
    } catch (nz::error err) {
      std::cout << (std::string)err << std::endl;
    }
  }
}
