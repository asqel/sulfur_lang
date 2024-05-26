#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/make_requested_vars.h"
#include "../include/make_context.h"
#include "../include/sulfur.h"

int inst_to_str(Instruction*i){
	if(i->type==inst_for_t){

		printf("For:[%s,%d]",CTX.requested_vars[i->value.fo->var_name], CTX.requested_vars[i->value.fo->endfor]);
		return 1;
	}
	if(i->type==inst_endfor_t){
		printf("Endfor:[%d]",i->value.endfor);
	}
	if(i->type==inst_expr_t){
	   printf("Expr:[]");
	}
	return 0;
}

void instruction_free_array(Instruction *code, int len){
	for(int i = 0; i < len; i++){
		instruction_free(code[i]);
	}
	if (code)
		free(code);
}

void instruction_free(Instruction code){
	switch (code.type){
		case inst_varset_t:
			free(code.value.vs->name);
			free(code.value.vs->type);
			free_ast(*code.value.vs->val);
			free(code.value.vs->val);
			break;
		case inst_endif:
		case inst_endifelse:
		case inst_jmp_t:
		case inst_endfor_t:
		case inst_endwhile_t:
			break;//nothing
		case inst_if_t:
			free_ast(*code.value.i->condition);
			free(code.value.i->condition);
			free(code.value.i);
			break;
		case inst_elif_t:
			free_ast(*code.value.el->condition);
			free(code.value.el->condition);
			free(code.value.el);
			break;
		case inst_while_t:
			free_ast(*code.value.wh->condition);
			free(code.value.wh->condition);
			free(code.value.wh);
			break;
		case inst_for_t:
			free_ast(*code.value.fo->end);
			free(code.value.fo->end);
			free_ast(*code.value.fo->start);
			free(code.value.fo->start);
			free(code.value.fo);
			break;
		case inst_expr_t:
			free_ast(*code.value.expr);
			free(code.value.expr);
			break;
		case inst_goto_t:
			free(code.value.goto_sec);
			break;
		case inst_section_t:
			free(code.value.section);
			break;
		case inst_return_t:
			free_ast(*code.value.ret);
			free(code.value.ret);
		case inst_pass_t:
		case inst_else_t:
		case inst_stop_t:
		case inst_proceed_t:
			break;//nothing
		case inst_funcdef_t:
			for (int i = 0; i < code.value.fc->args_len; i++) {
				free(code.value.fc->args[i]);
			}
			if (code.value.fc->args)
				free(code.value.fc->args);
			instruction_free_array(code.value.fc->code, code.value.fc->code_len);
			free(code.value.fc->info.name);
			free(code.value.fc);
			break;
		default:
			printf("Error: no way to free type %d\n"
					"please add it to instruction_free\n", code.type);
			exit(1);
	}
}
