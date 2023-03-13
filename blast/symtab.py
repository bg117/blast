"""Symbol table implementation for use by the interpreter.

This module contains the SymbolTable class, which is used by the interpreter to store
variables and their values. It is a simple implementation of a symbol table.
"""

from enum import IntEnum, auto
from typing import Any

class SymbolType(IntEnum):
    """The type of a symbol."""
    VARIABLE = auto()
    FUNCTION = auto()

class SymbolTable:
    """The symbol table for use by the interpreter."""

    def __init__(self, parent=None):
        """Initializes the symbol table."""
        self._symbols = {}
        self._parent = parent

    def __repr__(self):
        return f"{self._symbols!r}"
    
    def get(self, name, type):
        """Get the value of a symbol.

        Args:
            name (str): The name of the symbol.
            type (SymbolType): The type of the symbol.

        Returns:
            Any: The value of the symbol.

        Raises:
            KeyError: The symbol does not exist.
        """
        try:
            return self._symbols[(name, type)]
        except KeyError:
            if self._parent is not None:
                return self._parent.get(name, type)
            else:
                raise KeyError(f"Symbol {name!r} of type {type!r} does not exist")
    
    def set(self, name, type, value):
        """Set the value of a symbol.

        Args:
            name (str): The name of the symbol.
            type (SymbolType): The type of the symbol.
            value (Any): The value to set the symbol to.
        """
        self._symbols[(name, type)] = value
