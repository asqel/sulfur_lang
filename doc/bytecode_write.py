code = b'.Su.' # magical number 

code += b'\x02\x00\x00\x00\x00\x00\x00\x00' # nbr of constant

code += b'\x53println\x00'
code += b'\x53bonjour\x00'
code += b'\00\00\00\00\00\00'  # end of constants


code += b'\01\x00\x00\x00\x00\x00\x00\x00' # nbr of ast

code += b'\01\x00\x00\x00\x00\x00\x00\x00' # max stack len of this ast
code += b'\02\x00\x00\x00\x00\x00\x00\x00' # nbr of instruction
code += b'\x01\x02' + b'\x01\x00\x00\x00\x00\x00\x00\x00' #push constants @1
code += b'\x02' + b'\x00\x00\x00\x00\x00\x00\x00\x00' + b'\x01\x00\x00\x00\x00\x00\x00\x00' # func  name + nbr args

code += b'\x00\x00\x00\x00\x00\x00' # end of asts


code += b'\x01\x00\x00\x00\x00\x00\x00\x00' # nbr of instruction
code += b'\x04' + b'\x00\x00\x00\x00\x00\x00\x00\x00' # expr ast @0

code += b'\x00\x00\x00\x00\x00\x00' # end of instrcution

import os

f = open(os.path.dirname(__file__) + "/main.subc", "wb")

f.write(code)

f.close()
