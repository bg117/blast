from abc import ABC


class AST(ABC):
    """Base class for all AST nodes.

    This class is used to represent the abstract syntax tree of a BLAST
    program.  It is used by the parser and the interpreter.
    """
    pass


class ExprAST(AST):
    """Base class for all expression AST nodes.
    """
    pass


class BinaryExprAST(ExprAST):
    """AST node representing a binary expression.
    """
    def __init__(self, op, lhs, rhs):
        """Initialize a BinaryExprAST.

        Args:
            op (Token): The operator.
            lhs (ExprAST): The left-hand side expression.
            rhs (ExprAST): The right-hand side expression.
        """
        self.op = op
        self.lhs = lhs
        self.rhs = rhs


class UnaryExprAST(ExprAST):
    """AST node representing a unary expression.
    """
    def __init__(self, op, expr):
        """Initialize a UnaryExprAST.

        Args:
            op (Token): The operator.
            expr (ExprAST): The expression.
        """
        self.op = op
        self.expr = expr


class NumberExprAST(ExprAST):
    """AST node representing a number.
    """
    def __init__(self, val):
        """Initialize a NumberExprAST.

        Args:
            val (float): The value of the number.
        """
        self.val = val
