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

enum OP_CODES {
	OPC_END_EXEC, // used to stop the execution (may not be the last instruction)
	OPC_PASS,

	OPC_JMP,
	OPC_JMP_AT,
	OPC_JMP_AT_IF,
	OPC_JMP_AT_IFN,
	OPC_JMP_IF,
	OPC_JMP_IFN,

	OPC_CALL,
	OPC_R_CALL,
	OPC_RET,

	OPC_PUSH,
	OPC_PUSH_VAR, // stack : [..., var_name], arg : hash of the string
	OPC_POP,
	OPC_EMPTY_STACK,
	OPC_DUP, 
	OPC_SWAP,
	OPC_PUSH_I,
	OPC_PUSH_1B,
	OPC_PUSH_0B,
	OPC_PUSH_OUNT,
	OPC_PUSH_FLOAP,
	OPC_PUSH_STR, // arg : string_len, characters (not null terminated)

	OPC_CALL_FUNC,
	OPC_RET_FUNC,
	OPC_DEF_FUNC, // arg : end_func address
	OPC_END_FUNC,
	
	OPC_ADD,
	OPC_SUB,
	OPC_MUL,
	OPC_DIV,
	OPC_MOD ,
	OPC_FLDIV,
	OPC_AND,
	OPC_OR,
	OPC_POW,
	OPC_LSHIFT,
	OPC_RSHIFT,
	OPC_PUSH_COLON,
	OPC_PUSH_DOT,
	OPC_NOT,
	OPC_NEGATE,
	OPC_EQ,
	OPC_GREATER,
	OPC_LESS,
	OPC_GREAT_EQ,
	OPC_LESS_EQ,
	OPC_NEQ,

	OPC_ASSIGN,
	OPC_PLUS_ASSIGN,
	OPC_MINUS_ASSIGN,
	OPC_COL_ASSIGN,
	OPC_DOT_ASSIGN,

	OPC_FOR_JMP, // [..., end, start, 'i', hash('i')] arg : FOR addresse

	OPC_END_CODE = 0xFF
};
#define OPC_SIZE_JMP_IF 9 // 1B(opc) + 8B(address)
#define OPC_SIZE_JMP_IFN 9
#define OPC_SIZE_JMP 9
#define OPC_SIZE_FOR_JMP 9

#endif