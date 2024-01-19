#include "../../include/bytecode/bytecode.h"
#include <stdarg.h>

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

Bytecode_t bytecode_cat_new(Bytecode_t b1, Bytecode_t b2) {
	//Bytecode_t res;
	//res.len = b1.len + b2.len;
	//res.bytes = malloc(sizeof(char) * res.len);
	//memcpy(res.bytes, b1.bytes, b1.len);
	//memcpy(&(res.bytes[b1.len]), b2.bytes, b2.len);
	//return res;
}

void bytecode_cat(Bytecode_t *dest, Bytecode_t source) {
	//dest->bytes = realloc(dest->bytes, dest->len + source.len);
	//memcpy(&(dest->bytes[dest->len]), source.bytes, source.len);
	//dest->len += source.len;
}