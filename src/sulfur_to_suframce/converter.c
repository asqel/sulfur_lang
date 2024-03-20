#include "../../include/instruction.h"


S_sulfur_int GOTO_ID = 0;

S_sulfur_int *instruction_line_loc = NULL;

Instruction *add_labels(Instruction *inst, int *len) {
	
}


// will replace .line by a unique identifier
Instruction *inst_line_to_uid(Instruction *inst, int len) {
	
}

char *inst_to_suframce(Instruction *inst, int len){
	inst = add_labels(inst, &len);
}