from .parser import Parser
from .ast import *


class Interpreter:
    """The interpreter for the BLAST programming language.
    """
    def __init__(self, source: str):
        """Initialize a new Interpreter instance from the given source code.

        Args:
            source (str): The source code to interpret.
        """
        parser = Parser(source)
        self.ast = parser.parse()

    def __init__(self, parser: Parser):
        """Initialize a new Interpreter instance from the given parser.

        Args:
            parser (Parser): The parser to use.
        """
        self.ast = parser.parse()
    
    def __init__(self, ast: AST):
        """Initialize a new Interpreter instance from the given AST.

        Args:
            ast (AST): The AST to interpret.
        """
        self.ast = ast
