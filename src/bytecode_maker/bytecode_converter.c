#include "../../include/bytecode_maker/bytecode_converter.h"
#include "../../include/bytecode_maker/make_constants.h"
#include "../../include/instruction.h"
#include <stdarg.h>

Object *constants = NULL;
S_sulfur_int constants_len = 0;

Bytecode_t *asts = NULL;
S_sulfur_int asts_len = 0;

Bytecode_t *insts_bc = NULL;
S_sulfur_int insts_bc_len = 0;


Bytecode_t new_bytecode() {
	Bytecode_t res;
	res.bytes = malloc(1);
	res.len = 0;
	return res;
}

Bytecode_t *bytecode_append_char(Bytecode_t *code, int n, ...){
	va_list args;
	va_start(args, n);

	code->bytes = realloc(code->bytes, code->len + n);

	for (int i = 0; i < n; i++) {
		unsigned int arg = va_arg(args, unsigned int);
		code->bytes[code->len + i] = (unsigned char)arg;
	}

	va_end(args);
	code->len += n;
	return code;
}

Bytecode_t *bytecode_append_llint(Bytecode_t *code, S_sulfur_int value){
	code->bytes = realloc(code->bytes, code->len + 8);
	for(int i = 0; i < 8; i++)
		code->bytes[code->len + i] = (unsigned char)0;
	memcpy(&(code->bytes[code->len]), &value, sizeof(S_sulfur_int));
	code->len += 8;
	return code;
}

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n){
	x->bytes = realloc(x->bytes, x->len + n);
	for (int i = 0; i < n; i++){
		x->bytes[x->len++] = bytes[i];
	}
	return x;
}

void free_bytecode(Bytecode_t x){
	free(x.bytes);
}

Bytecode_t Constants_to_bytecode(Object *argv, int argc){
	//the 3 first will not be converted
	//they are suposed to be $i 0b 1b
	Bytecode_t code = new_bytecode();
	for (int i = 3; i < argc; i++){
		if (argv[i].type == Obj_ount_t){
			bytecode_append_char(&code, 1, (char) 0x49);
			bytecode_append_llint(&code, argv[i].val.i);
		}
		else if (argv[i].type == Obj_string_t){
			bytecode_append_char(&code, 1, (char) 0x53);
			bytecode_add_char_arr(&code, argv[i].val.s, strlen(argv[i].val.s) + 1);
		}
		else if (argv[i].type == Obj_floap_t){
			bytecode_append_char(&code, 1, (char) 0x46);
			double floap_val = (double)argv[i].val.f;
			memcpy(code.bytes + code.len, &floap_val, 8);
			code.len += 8;
		}
		else{
			printf("ERROR cant convert constant to bytecode type nbr %d arg %d\n", argv[i].type, i);
		}
	}
	return code;
}

Bytecode_t bytecode_cat_new(Bytecode_t b1, Bytecode_t b2) {
	Bytecode_t res;
	res.len = b1.len + b2.len;
	res.bytes = malloc(sizeof(char) * res.len);
	memcpy(res.bytes, b1.bytes, b1.len);
	memcpy(&(res.bytes[b1.len]), b2.bytes, b2.len);
	return res;
}

void bytecode_cat(Bytecode_t *dest, Bytecode_t source) {
	dest->bytes = realloc(dest->bytes, dest->len + source.len);
	memcpy(&(dest->bytes[dest->len]), source.bytes, source.len);
	dest->len += source.len;
}

S_sulfur_int add_bytecode_ast(Bytecode_t ast, S_sulfur_int *len) {
	Bytecode_t res = new_bytecode();
	bytecode_append_llint(&res, *len);
	bytecode_cat(&res, ast);
	free_bytecode(ast);
	asts_len++;
	asts = realloc(asts, sizeof(Bytecode_t) * asts_len);
	asts[asts_len - 1] = res;
	return asts_len - 1;
}

Bytecode_t make_bytecode_inst(Instruction inst) {
	S_sulfur_int temp = 0;
	Bytecode_t code = new_bytecode();
	if (inst.type == inst_pass_t)
		bytecode_append_char(&code, 1, '\x00');
	else if (inst.type == inst_for_t) {
		bytecode_append_char(&code, 1, '\x01');
		bytecode_append_llint(&code, add_constant_str(inst.value.fo->var_name));
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.fo->start, &temp), &temp));
		temp = 0;
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.fo->end, &temp), &temp));
		bytecode_append_llint(&code, inst.value.fo->endfor); 
	}
	else if (inst.type == inst_endfor_t) {
		bytecode_append_char(&code, 1, '\x02');
		bytecode_append_llint(&code, inst.value.endfor);
	}
	else if (inst.type == inst_endwhile_t) {
		bytecode_append_char(&code, 1, '\x02');
		bytecode_append_llint(&code, inst.value.endwhile);
	}
	else if (inst.type == inst_proceed_t)
		bytecode_append_char(&code, 2, '\x03', '\x01');
	else if (inst.type == inst_stop_t)
		bytecode_append_char(&code, 2, '\x03', '\x02');
	else if (inst.type == inst_proceed_t) {
		bytecode_append_char(&code, 2, '\x03', '\x03');
		bytecode_append_llint(&code, add_constant_str(inst.value.goto_sec));
	}
	else if (inst.type == inst_expr_t) {
		bytecode_append_char(&code, 1, '\x04');
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.expr, &temp), &temp));
	}
	else if (inst.type == inst_if_t) {
		bytecode_append_char(&code, 1, '\x05');
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.i->condition, &temp), &temp));
		bytecode_append_llint(&code, inst.value.i->endif_p);
	}
	else if (inst.type == inst_elif_t) {
		bytecode_append_char(&code, 1, '\x06');
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.el->condition, &temp), &temp));
		bytecode_append_llint(&code, inst.value.el->endif_p);
	}
	else if (inst.type == inst_else_t) {
		bytecode_append_char(&code, 1, '\x07');
		bytecode_append_llint(&code, inst.value.endif);
	}
	else if (inst.type == inst_endif) {
		bytecode_append_char(&code, 1, '\x08');
		bytecode_append_llint(&code, inst.value.endifelse);
	}
	else if (inst.type == inst_endifelse) {
		bytecode_append_char(&code, 1, '\x09');
		bytecode_append_llint(&code, inst.value.endifelse);
	}
	else if (inst.type == inst_return_t) {
		bytecode_append_char(&code, 1, '\x0A');
		bytecode_append_llint(&code, add_bytecode_ast(ast_to_bytecode(*inst.value.ret, &temp), &temp));
	}
	else if (inst.type == inst_section_t) {
		bytecode_append_char(&code, 1, '\x0B');
		bytecode_append_llint(&code, add_constant_str(inst.value.section));
	}
	return code;
}



Bytecode_t make_bytecode(Instruction *inst, int len) {
	constants = new_constants(&constants_len);
	insts_bc = malloc(sizeof(Bytecode_t) * len);
	insts_bc_len = len;
	for(int i = 0; i < len; i++)
		insts_bc[i] = make_bytecode_inst(inst[i]);

	Bytecode_t res = new_bytecode();

	bytecode_add_char_arr(&res, ".Su.", 4);

	//add constants
	bytecode_append_llint(&res, constants_len - 3);
	Bytecode_t consts_bytecode = Constants_to_bytecode(constants, constants_len);
	bytecode_cat(&res, consts_bytecode);
	free_bytecode(consts_bytecode);
	bytecode_add_char_arr(&res, "\0\0\0\0\0\0", 6);
	
	//add asts
	bytecode_append_llint(&res, asts_len);
	for (int i = 0; i < asts_len; i++) {
		bytecode_cat(&res, asts[i]);
		free(asts[i].bytes);
	}
	free(asts);
	bytecode_add_char_arr(&res, "\0\0\0\0\0\0", 6);

	//add instructions
	bytecode_append_llint(&res, insts_bc_len);
	for (int i = 0; i < insts_bc_len; i++) {
		bytecode_cat(&res, insts_bc[i]);
		free(insts_bc[i].bytes);
	}
	free(insts_bc);
	bytecode_add_char_arr(&res, "\0\0\0\0\0\0", 6);
	return res;
}