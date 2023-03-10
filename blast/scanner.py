class Scanner:
    """Scanner class for lexical analysis of the input stream.
    Uses regular expressions to recognize tokens.
    """

    def __init__(self, source: str):
        """Creates a new scanner instance.

        Args:
            source (str): The source code to be scanned.
        """
        self._source = source
        self._tokens = []
        self._start = 0
        self._current = 0
        self._line = 1