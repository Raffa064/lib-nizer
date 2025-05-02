# Nizer Library

A **tokenization and recursive descent parser (RDP)** library. It makes it easier to build custom language parsers by hand!

---

## Creating a Basic Parser

We’ll walk through the implementation of a simple **mathematical expression parser**.

You can find a full example with **evaluation** [here](./test/main.cpp).

---

## Defining Symbols

Before parsing, we need to define the **symbols** (tokens) of our language.  
Each symbol consists of:

- A **regular expression pattern**.
- A **flag**:  
  - `REGULAR` — default behavior (normal token).  
  - `SKIP` — ignored tokens (e.g., whitespace or comments).  
  - `MULTILINE` — spans across multiple lines.

Let’s define symbols for:

- Numbers
- Math operators
- Whitespace (skipped)

```cpp
SymbolList symbols;

Symbol NUM = symbols("[0-9]+", nz::REGULAR);       // Unsigned integers
Symbol OP  = symbols("[\\+\\-\\*/]", nz::REGULAR); // Math operators: + - * /
Symbol WS  = symbols("[ \n\r\t]+", nz::SKIP);      // Whitespace (ignored)
```

> [!TIP]
> You can use the built-in whitespace symbol with:  
> `Symbol WS = symbols(nz::sym::WS);`

---

## Parsing Rules

A parser rule is a function that consumes tokens and builds an **AST (Abstract Syntax Tree)** node.

Signature:

```cpp
AST* parseSomething(Consumer& consumer);
```

We’ll build the parser in three layers:

1. `factor` — the most basic unit (a number).
2. `term` — factors joined by `*` or `/`.
3. `expression` — terms joined by `+` or `-`.

---


```cpp

// factor: num
AST *parseFactor(Consumer &consumer) {
  Token num;

  if (consumer.consume({&num}, {NUM})) {
    AST& node = *new AST("factor");
    node["value"] = std::stoi(num.value);
    return &node;
  }

  throw nz::error("Syntax Error", "Non number factor", consumer.at());
}

// term: factor [('*' | '/') factor]*
AST *parseTerm(Consumer &consumer) {
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

// expression: term [('+' | '-') term]*
AST *parseExpression(Consumer &consumer) {
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

  if (consumer.has())
    throw nz::error("Dangling tokens", consumer.at());



  return left;
}
```

> [!IMPORTANT]
> `parseExpression` is the **entry rule** — it produces the root of the AST.

---

## Putting It All Together

Here’s how to parse an expression string:

```cpp
int main() {
  std::string exp = "10 * 2 / 3 - 4";

  token_vector tokens = nz::tokenize(symbols, exp);
  Consumer consumer(exp, tokens);

  AST* tree = parseExpression(consumer);
  std::cout << nz::ast_to_string(tree) << std::endl;
}
```

**Output:**
```
(op (left:op (left:op (left:factor) (right:factor)) (right:factor)) (right:factor))
```

**Visual AST:**
```
          (-)
         /   \
       (/)   (4)
      /   \
    (*)   (3) 
    / \
 (10) (2)
```
