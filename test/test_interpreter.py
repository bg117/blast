import unittest
from blast.interpreter import Interpreter
from blast.parser import Parser


class TestInterpreter(unittest.TestCase):
    AST = Parser(source="1 + (-2) * 3 / 4").parse()

    def test_evaluate(self):
        interpreter = Interpreter(ast=self.AST)
        self.assertEqual(interpreter.evaluate(), -0.5)

    def test_visit_binary_expr(self):
        interpreter = Interpreter(ast=self.AST)
        self.assertEqual(interpreter.visit_binary_expr(self.AST), -0.5)

    def test_visit_number_expr(self):
        interpreter = Interpreter(ast=self.AST)
        self.assertEqual(interpreter.visit_number_expr(self.AST.lhs), 1.0)
        self.assertEqual(interpreter.visit_number_expr(
            self.AST.rhs.lhs.rhs), 3.0)
        self.assertEqual(interpreter.visit_number_expr(self.AST.rhs.rhs), 4.0)

    def test_visit_unary_expr(self):
        interpreter = Interpreter(ast=self.AST)
        self.assertEqual(interpreter.visit_unary_expr(
            self.AST.rhs.lhs.lhs), -2.0)
