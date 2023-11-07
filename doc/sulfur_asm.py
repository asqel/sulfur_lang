
##types
define_const = "def_const"	#  1#
call_const = "call_const"	#  #1
call_var = "call_var"		#  !1
define_ast = "def_ast"		#  1@
call_ast = "call_ast"		#  @1
identifier = "id"			#  an_id
string = "str"				#  "bonjour"
integer = "int"				#  decimal / 0x / decimal negatif
float_t = "float"			#  3.14 / -5.0
separator = "sep"			#  ===  
line_end = "line_end"		#
operator = "op"				#  # @ !
semicolon = ";"				#  ;      (to comment everytghinf before)

id_acceptable = "abcdefghijklmnopqrstuvwxyz"
id_acceptable += "abcdefghijklmnopqrstuvwxyz".upper()
id_acceptable += "0123456789_"

class Token:
	def __init__(self, type : str, value : int | str, line : int) -> None:
		self.type = type
		self.value = value
		self.line = line
		self.to_delete = False

	def __str__(self) -> str:
		if self.value != "\n":
			return f'T_{self.type}[{self.value}]'
		return f'T_{self.type}[\\n]'

	def __repr__(self) -> str:
		return str(self)

def lexe(text : str) -> list[Token]:
	text += "\n"
	tokens : list[Token] = []
	p = 0
	l = len(text)
	line = 1
	while p < l:
		if text[p] == "=" :
			eq = "="
			p += 1
			while len(eq) != 3 and p < l and text[p] == '=':
				if text[p] != "=":
					break
				eq += "="
				p += 1
			if len(eq) == 3:
				tokens.append(Token(separator, "=", line))
				continue
			else:
				raise Exception(f"ERROR in lexer unexpected token, on line {line}")
		if text[p] == "/" and p + 1 < l and text[p + 1] in "/":
			while text[p] != '\n':
				p+=1
			continue
		if text[p] == '\n':
			tokens.append(Token(line_end, "\n", line))
			p +=1
			line += 1
			continue
		if text[p] in "\r\t \v":
			p += 1
			continue
		if text[p] == '0' and p + 1 < l and text[p + 1] in "Xx":
			number = ""
			for i in range(p + 2, l):
				if text[i] not in "1234567890ABCDEF":
					break
				number += text[i]
			if not number:
				raise Exception(f"ERROR in lexer expected numbers after 0x, on line {line}")
			for i in number:
				if i not in "1234567890ABCDEF":
					raise Exception(f"ERROR in lexer expected numbers after 0x, on line {line}")
			tokens.append(Token(integer, int("0x" + number, 16), line))
			p += 2 + len(number)
			continue

		if text[p] in '-1234567890.':
			number = ""
			while p < l and text[p] in '-1234567890.':
				number += text[p]
				p += 1
			if number.count('.') > 1:
				raise Exception(f"ERROR in lexer expected at most one dot in number, on line {line}")
			if number.count('-') > 1:
				raise Exception(f"ERROR in lexer expected at most one minus in number, on line {line}")
			if number.count('-') == 1 and number[0] != '-':
				raise Exception(f"ERROR in lexer expected the minus to be a start in number, on line {line}")
			if number.count('.') == 0:
				if (p < l and text[p] == "#"):
					tokens.append(Token(define_const, int(number), line))
					p += 1
				elif (p < l and text[p] == "@"):
					tokens.append(Token(define_ast, int(number), line))
					p += 1
				else:
					tokens.append(Token(integer, int(number), line))
			else:
				tokens.append(Token(float_t, float(number), line))
			continue
		if text[p] == '"':
			start = p + 1
			p += 1
			end = -1
			while p < l:
				if text[p] == '"':
					end = p
					break
				p += 1
			if end == -1:
				raise Exception(f"ERROR in lexer closing \" not found, on line {line}")
			tokens.append(Token(string, text[start : end], line))
			p+=1
			continue
		if text[p] == "'":
			start = p + 1
			end = -1
			while p < l:
				if text[p] == "'":
					end = p
					break
				p += 1
			if end == -1:
				raise Exception(f"ERROR in lexer closing \' not found, on line {line}")
			tokens.append(Token(string, text[start : end]))
			p += 1
			continue
		if text[p] in "#@!":
			tokens.append(Token(operator, text[p], line))
			p += 1
			continue
		if text[p] in ";":
			tokens.append(Token(semicolon, ";", line))
			p += 1
			continue
		if text[p] in id_acceptable and text[p] not in "0123456789":
			name = ""
			while text[p] in id_acceptable :
				name += text[p]
				p += 1
			tokens.append(Token(identifier, name, line))
		else:
			raise Exception(f"ERROR in lexer unexpected token, on line {line}")
	p = 0
	while p < len(tokens):
		if tokens[p].type == semicolon:
			k = p
			while k >= 0 and tokens[k].type != line_end:
				tokens[k].to_delete= True
				k -= 1
		p += 1

	p = 0
	while p < len(tokens):
		if tokens[p].to_delete:
			tokens.pop(p)
		else:
			p += 1

	p = 0
	while p < len(tokens):
		if tokens[p].type == operator and tokens[p].value == "#":
			if p + 1 < len(tokens) and tokens[p + 1].type == integer:
				tokens[p].type = call_const
				tokens[p].value = tokens[p + 1].value
				tokens.pop(p + 1)
				continue
			else:
				raise Exception(f"ERROR in lexer unexpected '#' on line {tokens[p].line}")
		if tokens[p].type == operator and tokens[p].value == "!":
			if p + 1 < len(tokens) and tokens[p + 1].type == integer:
				tokens[p].type = call_var
				tokens[p].value = tokens[p + 1].value
				tokens.pop(p + 1)
				continue
			else:
				raise Exception(f"ERROR in lexer unexpected '!' on line {tokens[p].line}")

		elif tokens[p].type == operator and tokens[p].value == "@":
			if p + 1 < len(tokens) and tokens[p + 1].type == integer:
				tokens[p].type = call_ast
				tokens[p].value = tokens[p + 1].value
				tokens.pop(p + 1)
				continue
			else:
				raise Exception(f"ERROR in lexer unexpected '@' on line {tokens[p].line}")
		else:
			p += 1

	return tokens

inst_def_const = "def_const"
inst_def_ast = "def_ast"

inst_PASS = "PASS"
inst_FOR = "FOR"
inst_END_FOR = "END_FOR"
inst_WHILE = "WHILE"
inst_END_WHILE = "END_WHILE"
inst_PROCEED = "PROCEED"
inst_STOP = "STOP"
inst_GOTO = "GOTO"
inst_JUMP = "JUMP"
inst_EXPR = "EXPR"
inst_IF = "IF"
inst_ELIF  = "ELIF"
inst_ELSE = "ELSE"
inst_ENDIF = "ENDIF"
inst_ENDIFELSE = "ENDIFELSE"
inst_RETURN  = "RETURN"
inst_SECTION = "SECTION"

class Inst:
	def __init__(self, type : str, args : list, line : int) -> None:
		self.type = type
		self.args = args
		self.line = line
	
	def __str__(self) -> str:
		return f'I_{self.type}{str(self.args)}'
	def __repr__(self) -> str:
		return str(self)
asts_singles = [
ast_add    := "add",
ast_sub    := "sub",
ast_mul    := "mul",
ast_div    := "div",
ast_fl_div := "fl_div",
ast_modulo := "modulo",
ast_power  := "power",
ast_or     := "or",
ast_and    := "and",
ast_not    := "not",
ast_negate := "negate",
ast_eq     := "eq",
ast_noteq  := "noteq",
ast_geq    := "geq",
ast_leq    := "leq",
ast_le     := "le",
ast_ge     := "ge",
ast_rshift := "rshift",
ast_lshift := "lshift",
ast_pop	   := "pop",
ast_call_func := "call_func",
ast_push_dot := "push_dot",
ast_assign_dot := "assign_dot",
ast_push_colon := "push_colon",
ast_assign_colon := "assign_colon",
ast_unpack := "unpack",
ast_call_ast := "call_ast",
ast_assign := "assign" 
]
ast_annonymous_function = "ano_func"
ast_annonymous_function_end = "ano_func_end"
ast_push = "push"


class ast_inst:
	def __init__(self, type : str, args : list, line : int) -> None:
		self.type = type
		self.args = args
		self.line = line
	
	def __str__(self) -> str:
		return f'AstI_{self.type}{str(self.args)}'
	def __repr__(self) -> str:
		return str(self)

def split_tokens(tokens : list[Token]) -> list[list[Token]]:
	res : list[list[Token]] = [[]]
	p = 0
	l = len(tokens)
	while p < l:
		if tokens[p].type == line_end:
			res.append([])
		else:
			res[-1].append(tokens[p])
		p += 1
	p = 0
	while p < len(res):
		if not res[p]:
			res.pop(p)
		else:
			p += 1
	return res

def parse_ast_inst(lines : list[list[Token]]):
	res : list[ast_inst] = []
	for i in lines:
		if len(i) == 1:
			if i[0].type == identifier and i[0].value.lower() in asts_singles:
				res.append(ast_inst(i[0].value.lower(), [], i[0].line))
		elif len(i) == 2:
			if i[0].type == identifier and i[0].value == ast_push:
				if i[1].type == call_const:
					res.append(ast_inst(ast_push, [2, i[1].value], i[0].line))
				elif i[1].type == call_var:
					res.append(ast_inst(ast_push, [1, i[1].value], i[0].line))
				else:
					raise Exception(f"ERROR in parser cannot push value on line {i[0].line}")
		else:
			raise Exception(f"ERROR in parser unexpected token on line {i[0].line}\n\t if it is an anonymous function, it is not supported yet")
	return res

def parse_inst(tokens : list[Token]) -> Inst:
	if tokens[0].type == identifier:
		inst = tokens[0].value.upper()
		if inst == inst_PASS:
			if len(tokens) != 1:
				raise Exception(f"ERROR instruction pass does not take arg, on line {tokens[0].line}")
			return Inst(inst_PASS, [], tokens[0].line) 
		if inst == inst_FOR:
			if len(tokens) != 5:
				raise Exception(f"ERROR instruction for only takes 4 arg, on line {tokens[0].line}")
			if (tokens[1].type, tokens[2].type, tokens[3].type, tokens[4].type) != (call_const, call_ast, call_ast, integer):
				raise Exception(f"ERROR instruction for only takes  constant ast ast integer, on line {tokens[0].line}")
			return Inst(inst_FOR, [tokens[i].value for i in range(1, 5)], tokens[0].line)
		if inst == inst_END_FOR:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction end_for only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != integer:
				raise Exception(f"ERROR instruction end_for only takes integer, on line {tokens[0].line}")
			return Inst(inst_END_FOR, [tokens[1].value], tokens[0].line)
		if inst == inst_WHILE:
			if len(tokens) != 3:
				raise Exception(f"ERROR instruction while only takes 2 arg, on line {tokens[0].line}")
			if (tokens[1].type, tokens[2].type) != (call_ast, integer):
				raise Exception(f"ERROR instruction while only takes ast integer, on line {tokens[0].line}")
			return Inst(inst_WHILE, [tokens[1].value, tokens[2].value], tokens[0].line)
		if inst == inst_END_WHILE:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction end_while only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != integer:
				raise Exception(f"ERROR instruction end_while only takes integer, on line {tokens[0].line}")
			return Inst(inst_END_WHILE, [tokens[1].value], tokens[0].line)
		if inst == inst_PROCEED:
			if len(tokens) != 1:
				raise Exception(f"ERROR instruction proceed only takes 0 arg, on line {tokens[0].line}")
			return Inst(inst_PROCEED, [], tokens[0].line)
		if inst == inst_STOP:
			if len(tokens) != 1:
				raise Exception(f"ERROR instruction stop only takes 0 arg, on line {tokens[0].line}")
			return Inst(inst_PROCEED, [], tokens[0].line)
		if inst == inst_GOTO:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction end_while only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != call_const:
				raise Exception(f"ERROR instruction end_while only takes constant, on line {tokens[0].line}")
			return Inst(inst_GOTO, [tokens[1].value], tokens[0].line)
		if inst == inst_JUMP:
			if len(tokens) != 3:
				raise Exception(f"ERROR instruction end_while only takes 2 arg, on line {tokens[0].line}")
			if tokens[1].type != call_ast and tokens[1].type != integer:
				raise Exception(f"ERROR instruction end_while only takes ast integer, on line {tokens[0].line}")
			return Inst(inst_GOTO, [tokens[1].value, tokens[2].value], tokens[0].line)
		if inst == inst_EXPR:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction expr only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != call_ast:
				raise Exception(f"ERROR instruction expr only takes ast, on line {tokens[0].line}")
			return Inst(inst_EXPR, [tokens[1].value], tokens[0].line)
		if inst == inst_IF:
			if len(tokens) != 3:
				raise Exception(f"ERROR instruction if only takes 2 arg, on line {tokens[0].line}")
			if (tokens[1].type, tokens[2].type) != (call_ast, integer):
				raise Exception(f"ERROR instruction if only takes ast integer, on line {tokens[0].line}")
			return Inst(inst_IF, [tokens[1].value, tokens[2].value], tokens[0].line)
		if inst == inst_ELIF:
			if len(tokens) != 3:
				raise Exception(f"ERROR instruction elif only takes 2 arg, on line {tokens[0].line}")
			if (tokens[1].type, tokens[2].type) != (call_ast, integer):
				raise Exception(f"ERROR instruction elif only takes ast integer, on line {tokens[0].line}")
			return Inst(inst_ELIF, [tokens[1].value, tokens[2].value], tokens[0].line)
		if inst == inst_ELSE:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction else only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != integer:
				raise Exception(f"ERROR instruction else only takes integer, on line {tokens[0].line}")
			return Inst(inst_ELSE, [tokens[1].value], tokens[0].line)
		if inst == inst_ENDIF:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction end_if only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != integer:
				raise Exception(f"ERROR instruction end_if only takes integer, on line {tokens[0].line}")
			return Inst(inst_ENDIF, [tokens[1].value], tokens[0].line)
		if inst == inst_ENDIFELSE:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction end_if_else only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != integer:
				raise Exception(f"ERROR instruction end_if_else only takes integer, on line {tokens[0].line}")
			return Inst(inst_ENDIFELSE, [tokens[1].value], tokens[0].line)
		if inst == inst_RETURN:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction return only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != call_ast:
				raise Exception(f"ERROR instruction return only takes ast, on line {tokens[0].line}")
			return Inst(inst_RETURN, [tokens[1].value], tokens[0].line)
		if inst == inst_SECTION:
			if len(tokens) != 2:
				raise Exception(f"ERROR instruction section only takes 1 arg, on line {tokens[0].line}")
			if tokens[1].type != call_const:
				raise Exception(f"ERROR instruction section only takes constant, on line {tokens[0].line}")
			return Inst(inst_SECTION, [tokens[1].value], tokens[0].line)
	raise Exception("ERROR parsing unexpected token on line " + str(tokens[0].line))
		
		
def parse(tokens : list[Token]) -> list[Inst]:
	instructions : list[Inst] = []
	lines = split_tokens(tokens)
	i = 0
	while i < len(lines):
		if len(lines[i]) == 2:
			if lines[i][0].type == define_const:
				if lines[i][1].type in [string, float_t, integer]:
					instructions.append(Inst(inst_def_const, [lines[i][0].value, lines[i][1].value], i))
					i += 1
					continue
				else:
					raise Exception("ERROR in parser define constant only accept string float or integer")
		elif len(lines[i]) == 1 and lines[i][0].type == separator:
			i += 1
			break
		else:
			raise Exception("ERROR in parser expected a separtor '===' between the constants and the asts")
	while i < len(lines):
		if len(lines[i]) == 1:
			if lines[i][0].type == define_ast:
				content = []
				for k in range(i + 1, len(lines)):
					if len(lines[k]) == 1 and (lines[k][0].type == define_ast or lines[k][0].type == separator):
						break
					content.append(lines[k])
				instructions.append(Inst(define_ast, [lines[i][0].value] + parse_ast_inst(content), i))
				i += len(content) + 1
				continue
		if len(lines[i]) == 1 and lines[i][0].type == separator:
			i += 1
			break
		else:
			raise Exception("ERROR in parser expected a separtor '===' between the ast and the instructions")
	while i < len(lines):
		instructions.append(parse_inst(lines[i]))
		i += 1
		
	return instructions

import struct as st

def build_instructions(insts : list[Inst]) -> bytes:
	res = b''
	res += st.pack("<q", len(insts))
	for i in insts:
		if i.type == inst_PASS: res += b'\x00'
		elif i.type == inst_FOR:
			res += b'\x01'
			for k in i.args:
				res +=  st.pack("<q", i)
		elif i.type == inst_END_FOR: res += b'\x02' + st.pack("<q", i.args[0])
		elif i.type == inst_WHILE: res += b'\x0f' + st.pack("<q", i.args[0]) + st.pack("<q", i.args[1])
		elif i.type == inst_END_WHILE: res += b'\x02' + st.pack("<q", i.args[0])
		elif i.type == inst_PROCEED: res += b'\x03\x01'
		elif i.type == inst_STOP: res += b'\x03\x02'
		elif i.type == inst_GOTO: res += b'\x03\x03' + st.pack("<q", i.args[0])
		elif i.type == inst_JUMP: res += b'\x03\x04' + st.pack("<q", i.args[0]) + st.pack("<q", i.args[1])
		elif i.type == inst_EXPR: res += b'\x04' + st.pack("<q", i.args[0])
		elif i.type == inst_IF: res += b'\x05' + st.pack("<q", i.args[0]) + st.pack("<q", i.args[1])
		elif i.type == inst_ELIF: res += b'\x06' + st.pack("<q", i.args[0]) + st.pack("<q", i.args[1])
		elif i.type == inst_ELSE: res += b'\x07' + st.pack("<q", i.args[0])
		elif i.type == inst_ENDIF: res += b'\x08' + st.pack("<q", i.args[0])
		elif i.type == inst_ENDIFELSE: res += b'\x09' + st.pack("<q", i.args[0])
		elif i.type == inst_RETURN: res += b'\x0a' + st.pack("<q", i.args[0])
		elif i.type == inst_SECTION: res += b'\x0b' + st.pack("<q", i.args[0])
	return res
def build_ast_inst(inst : ast_inst) -> bytes:
	if inst.type == ast_add: return b'\x05'
	if inst.type == ast_sub: return b'\x06'
	if inst.type == ast_mul: return b'\x07'
	if inst.type == ast_div: return b'\x08'
	if inst.type == ast_fl_div: return b'\x09'
	if inst.type == ast_modulo: return b'\x0A'
	if inst.type == ast_power: return b'\x0B'
	if inst.type == ast_or: return b'\x0C'
	if inst.type == ast_and: return b'\x0D'
	if inst.type == ast_not: return b'\x0E'
	if inst.type == ast_negate: return b'\x0F'
	if inst.type == ast_eq: return b'\x10'
	if inst.type == ast_noteq: return b'\x11'
	if inst.type == ast_geq: return b'\x12'
	if inst.type == ast_leq: return b'\x13'
	if inst.type == ast_le: return b'\x14'
	if inst.type == ast_ge: return b'\x15'
	if inst.type == ast_rshift: return b'\x16'
	if inst.type == ast_lshift: return b'\x17'
	if inst.type == ast_assign : return b'\xF0'
	if inst.type == ast_push_dot : return b'\xF1'
	if inst.type == ast_assign_dot: return b'\xF2'
	if inst.type == ast_push_colon: return b'\xF3'
	if inst.type == ast_assign_colon: return b'\xF4'
	if inst.type == ast_unpack: return b'\xF5'
	if inst.type == ast_call_ast: return b'\xF6'
	if inst.type == ast_pop: return b'\x04'
	if inst.type == ast_annonymous_function:
		raise Exception("ERROR annonymous function not supported yet")
	if inst.type == ast_call_func: return b'\x02'
	if inst.type == ast_push: return b'\x01' + (b'\x01' if inst.args[0] == 1 else b'\x01') + st.pack("<q", inst.args[1])

def build_ast(insts : list[Inst]) -> bytes:
	nbr_asts = max(len(insts), max([i.args[0] for i in insts]))
	asts : list[Inst] = [None for i in range(nbr_asts)]
	for i in insts:
		asts[i.args[0]] = i
	res = b''
	res += st.pack("<q", nbr_asts)
	for i in asts:
		res += st.pack("<q", len(i.args) - 1)
		for k in range(len(i.args) - 1):
			res += build_ast_inst(i.args[k + 1])
	return res


def build_constants(insts : list[Inst]) -> bytes:
	nbr_constants = max(len(insts), max([i.args[0] for i in insts])) + 1
	constants : list[Inst] = [None for i in range(nbr_constants)]
	for i in insts:
		if i.args[0] < 3:
			raise Exception("ERROR constants index can only be greater or equal to 3")
		constants[i.args[0]] = i
	res = b''
	res += st.pack("<q", nbr_constants - 3)
	constants.pop(0)
	constants.pop(0)
	constants.pop(0)
	for i in constants:
		if i is not None:
			if type(i.args[1]) == str:
				res += b'\x53'
				for i in i.args[1]:
					res += i.encode()
				res += b'\x00'
			elif type(i.args[1]) == int:
				res += b'\x49'
				res += st.pack("<q", i.args[1])
			elif type(i.args[1]) == float:
				res += b'\x46'
				res += st.pack("<d", i.args[1])
		else:
			res += b'\x49'
			res += st.pack("<q", 0)
	return res


def build_asm(text : str):
	tokens : list[Token] = lexe(text)
	instructions : list[Inst] = parse(tokens)
	constants = []
	asts = []
	insts = []
	for i in instructions:
		if i.type != define_const:
			continue
		constants.append(i)
	for i in instructions:
		if i.type != define_ast:
			continue
		asts.append(i)
	for i in instructions:
		if i.type != define_const and i.type != define_ast:
			insts.append(i)
	code = b'.Su.'
	code += build_constants(constants)
	code += b'\0\0\0\0\0\0'
	code += build_ast(asts)
	code += b'\0\0\0\0\0\0'
	code += build_instructions(insts)
	code += b'\0\0\0\0\0\0'
	return code

import os
code = b''
with open(os.path.dirname(__file__)+"/test.suasm","r") as f:
	code = build_asm(f.read())
if not os.path.exists(os.path.dirname(__file__)+"/test.subc"):
	with open(r"C:\Users\Z240\Documents\GitHub\sulfur_lang\doc\test.subc","xb") as f:
		f.write(code)
else:
	with open(r"C:\Users\Z240\Documents\GitHub\sulfur_lang\doc\test.subc","wb") as f:
		f.write(code)
