#ifndef BYTECODE_OP_CODES_H
#define BYTECODE_OP_CODES_H

#define MAGICAL_NUMBER0 '.'
#define MAGICAL_NUMBER1 'S'
#define MAGICAL_NUMBER2 'u'
#define MAGICAL_NUMBER3 '.'
#define MAGICAL_NUMBER_AS_STR ".Su."

#define OUNT_MAGIC_NUM 'I' 
#define STRING_MAGIC_NUM 'S'
#define FLOAP_MAGIC_NUM 'F'
#define FIRST_INST_ADRESS_LOCATION 0x04
/*
representation of the stack :
	[bottom, ..., top]
reprenstation of the call_stack:
	{bottom, ..., top}



END_EXEC			|   stop the execution
PASS				|	do nothing
--------------------|-----------------
# when jumping to an adress the instruction will be executed before incrementing the instruction pointer
JMP (addr)			|	jmp to address addr
JMP_IF (addr)		|	jmp to address addr if the top of the stack is true (if stack is empty : error)
JMP_IFN (addr)		|	jmp to address addr if the top of the stack is false (empty stack = false)
R_JMP (offset)		|	same as jmp but relative 
R_JMP_IF (offset)	|	...
R_JMP_IFN (offset)	|	...

# same before but the address or the offest will be taken from the top of the 
	stack and the condtion if any from the second top of stack
# if stack doesn't contain at least 2 object an error will be raised
JMP					|	...
JMP_IF				|	...
JMP_IFN				|	...
R_JMP				|	...
R_JMP_IF			|	...	
R_JMP_IFN			|	...

CALL (addr)			|  store the current address in the call_stack and jump to address addr
RET					|  jump to address at top of call_stack and pop it (call_stack empty = error) 

FUNC_DEF_O  (addr)	|   [..., arg3, arg2, arg1, nbr, func_name]
					|	create an object in the current scope of type function
					|	with name func_name
					|	with a nbr args and their names taken from the stack 
					|	the mod of the function is only (= funcction takes only nbr args)
					|	addr is the address of the END_FUNC
FUNC_DEF_P (addr)	|   [..., arg3, arg2, arg1, nbr, func_name]
					|	create an object in wthe current scope of type function
					|	with name func_name
					|	with a nbr args and their names taken from the stack 
					|	the mod of the function is plus (= function takes at least nbr - 1 args)
					|	addr is the address of the END_FUNC
RET_FUNC			|	stop the execution of the functionn and return
					|		every function should end with a return
					|		if not sulfur might add a push_nil; ret_func;
					|
END_FUNC (addr)		|	marks the end of the function, addr is the address of the FUNC_DEF
CALL_FUNC			|	[..., arg3, arg2, arg1, nbr, func] raise an error if func is not a functon
					|   
					|
POP					|	remove the top of the stack (do nothing if stack is empty)
PUSH (addr)			|   push the value of the cconstant at addr on top of the stack
					|		raise error if constant at addr is not recognized
PUSH_VAR			|	[...., var_name] push on top of the stacck the value of the var var_name and remove var_name from the stack
					|		var_name must be a string 
					|		if stack is empty or var_name doesn't correspond to any variable, raise error
PUSH_NIL			|	push the nil value on to the stack
PUSH_0B				|	push the value of 0b on to the stack
PUSH_1B				|	push the value of 1b on to the stack
PUSH_IM				|	push the value of the imaginary unit ($i) on to the stack
PUSH_COL			|	[..., obj, idx]  push the idx member of obj onto the stack and remove them
PUSH_DOT			|	[..., obj, mem]  push the mem member of obj onto the stack and remove them
						|	mem must be a string
ADD
SUB
MUL
DIV
MOD
FLDIV
AND
OR
POW
LSHIFT
RSHIFT
COLON
DOT
NOT
NEGATE
EQ					|	[..., val, var_name] assign the  value val into the var var_name of the ccurrent scope or global scope
GREATER
LESS
GREAT_EQ
LESS_EQ
NEQ
ASSIGN
PLUS_ASSIGN			|	[..., val, var_name] add the  value val to the var var_name of the current scope or global scope
MINUS_ASSIGN		|	[..., val, var_name] substract the  value val of the var var_name of the current scope or global scope
ASSIGN_COL			|	[..., val, obj, idx] assigne the value val to member idx of obj
ASSIGN_DOT			|	[..., val, obj, mem] assigne the value val to member mem of obj
					|		mem must be a string
SWAP				|	[..., obj2, obj1] swap obj2 and obj1
R_CALL (offset)		|  same as CALL but jump to the current address + offset

*/
/*
.subc files
at 0x0 	: the magical number on 4 bytes '.Su.'
at 0x04 : on 8byte the address of the first instruction
at 0x0C : constants and instructions

at the start the interpreter will jmp to the address of the first instruction
the adress of the first instruction is at 0x04 on 8 bytes in little endian


*/
#define OPC_END_EXEC 0x00 // used to stop the execution (may not be the last instruction)
#define OPC_PASS 0x01
#define OPC_JMP 0x02
#define OPC_JMP_AT 0x03
#define OPC_JMP_AT_IF 0x04
#define OPC_JMP_AT_IFN 0x05

#define OPC_JMP_IF 0x06
#define OPC_JMP_IFN 0x07


#define OPC_CALL 0x08
#define OPC_RET 0x09
#define OPC_PUSH 0xA
#define OPC_PUSH_VAR 0x0B
#define OPC_POP 0x0C
#define OPC_CALL_FUNC 0x0D
#define OPC_RET_FUNC 0x0E
#define OPC_DEF_FUNC 0x0F
#define OPC_DUP 0x10
#define OPC_SWAP 0x11
#define OPC_PUSH_I 0x12 // push $i
#define OPC_PUSH_1B 0x13
#define OPC_PUSH_0B 0x14
#define OPC_PUSH_OUNT 0x15 // 1B + 8B
#define OPC_PUSH_FLOAP 0x16

#define OPC_ADD 0xD0
#define OPC_SUB 0xD1
#define OPC_MUL 0xD2
#define OPC_DIV 0xD3
#define OPC_MOD 0xD4
#define OPC_FLDIV 0xD5
#define OPC_AND 0xD6
#define OPC_OR 0xD7
#define OPC_POW 0xD8
#define OPC_LSHIFT 0xD9
#define OPC_RSHIFT 0xDA
#define OPC_COLON 0xDB
#define OPC_DOT 0xDC

#define OPC_NOT 0xDD
#define OPC_NEGATE 0xDE
#define OPC_NEGATE 0xDF
#define OPC_EQ 0xE0
#define OPC_GREATER 0xE1
#define OPC_LESS 0xE2
#define OPC_GREAT_EQ 0xE3
#define OPC_LESS_EQ 0xE4
#define OPC_NEQ 0xE5
#define OPC_ASSIGN 0xE6
#define OPC_PLUS_ASSIGN 0xE7
#define OPC_MINUS_ASSIGN 0xE8
#define OPC_R_CALL 0xE9

#define OPC_END_CODE 0xFF // the last instruction (must be the last byte of the file)

#define SIZE_END 1
#define SIZE_PASS 1
#define SIZE_JMP 9
#define SIZE_JMP_IF 9
#define SIZE_JMP_IFN 9
#define SIZE_JMP_AT 1
#define SIZE_JMP_AT_IF 1
#define SIZE_JMP_AT_IFN 1
#define SIZE_CALL 1
#define SIZE_RET 1
#define SIZE_PUSH 9
#define SIZE_PUSH_VAR 1
#define SIZE_POP 1
#define SIZE_CALL_FUNC 1
#define SIZE_RET_FUNC 1
#define SIZE_DEF_FUNC 10

#endif