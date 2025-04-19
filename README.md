# Nizer Library

A **tokenization and recursive descent parser (RDP)** library. It makes it easier to build custom language parsers by hand!

## Creating a Basic Parser

In the above examples, we’ll implement a simple mathematical expression **parser**. 

You can see fully implemented code (with evaluation) [here](./test/main.cpp).

### Symbols

First, we need to define our grammar symbols. These symbols are labels that identify **meaningful words** in our language.

A symbol consists of two main components: a regular expression pattern and a bit flag value. The flag is usually set to `REGULAR`, meaning it’s a normal token. Other options include `SKIP` (to ignore the token, useful for things like comments) and `MULTILINE` (used to match tokens that span multiple lines).

For this parser, we’ll define three symbols: one for numbers, one for operators, and one special symbol for whitespace (usually called `WS`, short for *white space*).

To define these symbols, we use a `SymbolList`, which behaves like a callable object to register new symbols:

```cpp
SymbolList symbols; // Our symbol list

Symbol NUM = symbols("[0-9]+", REGULAR);    // Unsigned integers
Symbol OP  = symbols("[\\+\\-\\*/]", REGULAR); // Math operators: + - * /
Symbol WS  = symbols("[ \n\r\t]+", SKIP);   // Whitespace (skipped)
```

> [!TIP]
> Since whitespace is common in most languages, you can use the built-in `NizerSymbols::WS`:
> ```cpp 
> Symbol WS = symbols(NizerSymbols::WS);
> ```

Now that we’ve defined our tokens, we need to describe how they form valid expressions.

---

### Parsing Rules

A parser rule is a function that takes a stream of tokens and builds a tree structure called an **AST (Abstract Syntax Tree)**. It follows this signature:

```cpp
AST* parseSomething(Nizer&, Consumer& consumer) { ... }
```

There's a macro for creating a parser rule, that makes it simpler, and allows to use some cool Nizer features:

```cpp
parser_rule(parseSomething) { ... }
```

These functions are the heart of a **recursive descent parser**. The "descendant" part will make more sense shortly.

To parse math expressions, we’ll create three parsing functions, each representing a level of abstraction in mathematical operations.

**But what is a math expression, really?**

We’re not concerned with the individual numbers or symbols, but with how operations are grouped and prioritized.

- The most basic element of an expression is a **factor** (e.g., a single number).
- A collection of factors connected by `*` or `/` forms a **term**.
- A collection of terms connected by `+` or `-` forms an **expression**.

Example:  
For the input `10 + 2 * 3 - 4`, we can break it down like this:

```
factor + factor * factor - factor
```

Then abstract it further:

```
term + term - term
```

The deeper we go, the more abstract and simplified the structure becomes—that’s the essence of a recursive descent parser.

Here’s how we implement it:

```cpp
AST* parseFactor(Nizer&, Consumer& consumer) {
  Token num;

  if (consumer.consume({&num}, {NUM})) {
    AST& node = *new AST("factor");
    node["value"] = std::stoi(num.value);
    return &node;
  }

  throw nizer_error("Invalid token at {}", {consumer.at()});
}
```

---

```cpp
// term = factor [('*' | '/') factor]*
AST* parseTerm(Nizer&, Consumer& consumer) {
  AST* left = parseFactor(consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\*/]")})) {
    AST* right = parseFactor(consumer);

    AST& node = *new AST("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  return left;
}
```

---

```cpp
// expression = term [('+' | '-') term]*
AST* parseExpression(Nizer&, Consumer& consumer) {
  AST* left = parseTerm(consumer);

  Token op;
  while (consumer.consume({&op}, {OP("[\\+\\-]")})) {
    AST* right = parseTerm(consumer);

    AST& node = *new AST("op");
    node["op"] = op.value;
    node["left"] = left;
    node["right"] = right;

    left = &node;
  }

  return left;
}
```

> [!TIP]
> The `parseExpression` function is our **starting rule**. It produces the root node of the AST.

---

### Parsing an Expression

To finalize the parser, we pass the symbol list and the starting rule (`parseExpression`) to the `Nizer` class. After that, we can parse any string containing a math expression:

```cpp
int main(int argc, char** argv) {
  Nizer nizer(symbols, parseExpression);

  AST* tree = nizer.parse("10 * 2 / 3 - 4");

  std::cout << ast_to_string(tree) << std::endl;
}
```

**Output:**
```
(op (left:op (left:factor) (right:op (left:factor) (right:factor))) (right:factor))
```

Which can be visualized as:

```
          (-)
         /   \
       (+)    (4)
      /   \
   (10)   (*)
         /   \
       (2)   (3)
```
