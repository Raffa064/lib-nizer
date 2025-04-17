# Lib Nizer

A tokenization and RDP (Recursive Descendant Parser) library. Makes it easier to create custom language parsers by hand!

# Basics
```cpp
// Ommitting includes for simplicity

SymbolList symbols;

Symbol OP = symbols("[\\+\\-]+", REGULAR);
Symbol NUM = symbols("[0-9]+", REGULAR);
Symbol WS = symbols("[ \n\r\t]+", SKIP);

AST* parseProg(Consumer& consumer) {
  AST &root = *new AST("prog");
  // Start parsing from here

  return &root;
}

int main(int argc, char** argv) {
  Nizer nizer(symbols, parseProg);

  AST *tree = nizer.parser("10 + 10");

  std::cout << ast_to_string(tree) << std::endl; // (prog ...)
}
```