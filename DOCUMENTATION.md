# Documentation

## Modules

- `blast.scanner`: BLAST scanner
- `blast.parser`: BLAST parser
- `blast.interpreter`: BLAST interpreter
- `blast.ast`: BLAST AST nodes
- `blast.token`: BLAST tokens and token types

### `blast.scanner`

#### `Scanner` class

- `__init__(self, source: str)`: initialize scanner with source code
- `scan_tokens(self) -> list[Token]`: scan source code into tokens

### `blast.parser`

#### `Parser` class

- `__init__(self, source: str, tokens: list[Token])`: initialize parser with either source code or tokens
- `parse(self) -> AST`: parse source code into abstract syntax tree

### `blast.interpreter`

#### `Interpreter` class

- `__init__(self, source: str, parser: Parser, ast: AST)`: initialize interpreter with either source code, parser, or AST
- `evaluate(self)`: evaluate source code into result

### `blast.ast`

- `AST`: abstract syntax tree node
- `ExprAST`: expression AST node
- `BinaryExprAST`: binary expression AST node
- `UnaryExprAST`: unary expression AST node
- `NumberExprAST`: number expression AST node
- `StringExprAST`: string expression AST node

### `blast.token`

#### `Token` class

- `__init__(self, type: TokenType, lexeme: str`: initialize token with type, lexeme
- `__str__(self) -> str`: string representation of token
- `__repr__(self) -> str`: string representation of token
- `__eq__(self, other: Token) -> bool`: equality of token
- `__hash__(self) -> int`: hash of token
- `type`: token type
- `lexeme`: token lexeme

#### `TokenType` enum

Enum of token types. See [`blast.token`](blast/token.py) for values.
