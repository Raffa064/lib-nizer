#include <iostream>
#include <token.h>
#include <vector>

void print_tokens(std::vector<Token> &tokens) {
  for (Token t : tokens) {
    std::cout << t.typeId << ":" << t.value << ", ";
  }

  std::cout << std::endl;
}
