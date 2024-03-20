//#include "../instruction.h"
//
//
//int next_uid = 0;
//
//#define get_uid() (next_uid++)
//
//
//
//Instruction	*finish_instrcutions(Instruction *code, int *instruction_len) {
//	// do uid
//	// uid will be stored in .line
//	for (int i = 0; i < *instruction_len; i++)
//		code[i].line = get_uid();
//	for (int i = 0; i < *instruction_len; i++) {
//		if (code[i].type == inst_funcdef_t) {
//			code[i].value.fc->code = finish_instrcutions(code[i].value.fc->code, &code[i].value.fc->code_len);
//		}
//	}
//	for (int i = 0; i <)
//
//}