from abc import ABC


class AST(ABC):
    """Base class for all AST nodes.

    This class is used to represent the abstract syntax tree of a BLAST
    program.  It is used by the parser and the interpreter.
    """

    def __repr__(self):
        raise NotImplementedError("Cannot repr a base AST node")

    def accept(self, visitor):
        raise NotImplementedError("Cannot accept from a base AST node")


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

    def __repr__(self):
        return f"<{self.op!r} {self.lhs!r} {self.rhs!r}>"

    def accept(self, visitor):
        return visitor.visit_binary_expr(self)


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

    def __repr__(self):
        return f"<{self.op!r} {self.expr!r}>"

    def accept(self, visitor):
        return visitor.visit_unary_expr(self)


class NumberExprAST(ExprAST):
    """AST node representing a number.
    """

    def __init__(self, val):
        """Initialize a NumberExprAST.

        Args:
            val (int): The value of the number.
        """
        self.val = val

    def __repr__(self):
        return f"<{self.val!r}>"

    def accept(self, visitor):
        return visitor.visit_number_expr(self)


class StringExprAST(ExprAST):
    """AST node representing a string.
    """

    def __init__(self, val):
        """Initialize a StringExprAST.

        Args:
            val (str): The value of the string.
        """
        self.val = val

    def __repr__(self):
        return f"<{self.val!r}>"

    def accept(self, visitor):
        return visitor.visit_string_expr(self)
