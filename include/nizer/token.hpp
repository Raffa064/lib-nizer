#pragma once

#include <string>
#include <vector>

struct Token {
  int typeId;
  std::string value;
  int index;
};

using token_vector = std::vector<Token>;

void print_tokens(token_vector &);
