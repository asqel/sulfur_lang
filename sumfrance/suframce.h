#ifndef SUFRAMCE_H
#define SUFRAMCE_H

typedef struct {
	char type;
	union {
		char c;
		char *s;
	}data;
	
}Suframce_token;


/*

ADDR : 
	+1 will be the address of the next instruction
		(can throw a compile bytecode ERROR)
	-2 will be the address of the second before insttruction
		(can throw a compile bytecode ERROR)
	*1  will be the address of the first instruction
	*2  will be the address of the second instruction
	^  will be the address of the last instruction
	$  will be the address of the current instruction


SEC:
	(ex: this_is_a_section::)
		sections (or label) are used by the jmps and calls
		ex:
			loop::
			//this will continue forever
			jmp loop
	
PRIMITIVE:
	int
	float
	str
	bool
	imaginary unit

ID:
	an identifier like: my_var
DEF_CALL:
	!ID : !my_def

call [SEC / ADDR / int]
JMP  [SEC / ADDR]

PUSH [PRIMITIVE / ADDR / ID / DEF_CALL]





*/


#endif