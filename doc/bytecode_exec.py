import struct


CONSTANTS_START = 4 #location of nbr of constants

code = b'.Su.'
code += b'\x03\x00\x00\x00\x00\x00\x00\x00'
code += b'\x49\x42\x00\x00\x00\x00\x00\x00\x00'
code += b'\x53salut\x00'
code += b'\x53comment?\x00'


class Object:
    def __init__(self, _type : str, value) -> None:
        self.type = _type
        self.value = value
    def __str__(self) -> str:
        return f"T_{self.type}[{self.value}]"
    def __repr__(self) -> str:
        return str(self)

nil_obj = Object("nil", 0)
im_unit = Object("complex", 1j)

constants = []
Asts = []

code_len = len(code)
print(code_len)
def make_constants():
    global constants
    p = CONSTANTS_START
    if p + 7 >= code_len:
        raise Exception("ERROR in bytecode cannot find nbr of constants")
    nbr_constants = struct.unpack("<Q", code[p : p+8])[0]
    p += 8

    while p < code_len:
        if code[p] == 0x49:
            if p + 8 >= code_len:
                raise Exception("ERROR in bytecode in constants")
            constants.append(Object("ount", struct.unpack("<Q", code[p + 1 : p + 9])[0]))
            p += 9
            continue
        if code[p] == 0x53:
            s = ""
            p += 1
            while p < code_len and code[p] != 0x00:
                s += chr(code[p])
                p += 1
            constants.append(Object("string", s))
            p += 1
            continue
        p +=1
    
    if len(constants) != nbr_constants:
        raise Exception(f"ERROR bytecode constant section incorrect nbr of constants")

make_constants()
print(constants)