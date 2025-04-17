#pragma once

#include <string>

struct Token {
  int typeId;
  std::string value;
  int index;
};

void print_tokens(std::vector<Token> &);
