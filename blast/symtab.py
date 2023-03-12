class SymbolTable:
    """The symbol table for use by the interpreter.
    """

    def __init__(self):
        self._symbols = {} # _symbols: {name: value}

    def __repr__(self):
        return f"{self._symbols!r}"
    
    def __getitem__(self, name):
        return self._symbols[name]
    
    def __setitem__(self, name, value):
        self._symbols[name] = value