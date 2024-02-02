#include "../../include/instruction.h"


S_sulfur_int GOTO_ID = 0;

S_sulfur_int *instruction_line_loc = NULL;


char *jmp_unique_label(int line) {
	// label : L_uid_line
	// line : instruction index
	S_sulfur_int uid = GOTO_ID++;
	int malloc_len = sizeof(char) * (1 + (int)log10(uid) + 1 + (int)log10(line) + 4);
	char *res = malloc(malloc_len);
	memset(res, 0,  malloc_len);
	sprintf(res, "L_%d_%d", uid, line);
	return realloc(res, sizeof(char) * (1 + strlen(res)));


}

Instruction *add_labels(Instruction *inst, int *len) {
	
}

char *inst_to_suframce(Instruction *inst, int len){
	inst = add_labels(inst, &len);
}