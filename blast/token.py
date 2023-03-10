class Token:
    def __init__(self, type, lexeme):
        self.type = type
        self.lexeme = lexeme

    def __repr__(self):
        return f"{self.type} {self.lexeme}"
    
    def __str__(self):
        return self.__repr__()
    
    def __eq__(self, other):
        return self.type == other.type and self.lexeme == other.lexeme
    
    def __hash__(self):
        return hash((self.type, self.lexeme))
