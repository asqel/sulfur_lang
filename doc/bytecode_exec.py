code : bytes = bytes([0,0])

class Object:
    def __init__(self, _type : str, value) -> None:
        self.type = _type
        self.value = value
    def __str__(self) -> str:
        return f"T_{self.type}[{self.value}]"

nil_obj = Object("nil", 0)
im_unit = Object("complex", 1j)

constants = []
Asts = []

code_len = len(code)

def make_constants():
    p = 0
    while p < code_len:
