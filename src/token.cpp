#include <iostream>
#include <nizer.hpp>

void print_tokens(token_vector &tokens) {
  for (Token t : tokens) {
    std::cout << t.typeId << ":" << t.value << ", ";
  }

  std::cout << std::endl;
}
