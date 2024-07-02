#include "../include/make_jmp_links.h"
#include "../include/utilities.h"



Instruction *make_jmp_links(Instruction *code, int len) {
	int *loops = NULL;
	int loop_count = 0;
	for(int i = 0; i < len; i++) {
		if (code[i].type == inst_goto_t) {
			int n = -1;
            //search down
            for(int k = i + 1; k < len; k++){
                if(code[k].type == inst_section_t){
                    if(!strcmp(code[i].value.goto_sec, code[k].value.section)){
                        n = k;
                        break;
                    }
                }
            }
            if(n == -1){
                //search up
                for(int k = i - 1; k >= 0; k--){
                    if(code[k].type == inst_section_t){
                        if(!strcmp(code[i].value.goto_sec, code[k].value.section)){
                            n = k;
                            break;
                        }
                    }
                }
            }
            if(n == -1){
                PRINT_ERR("section %s doesnt exists\n", code[i].value.goto_sec);
                exit(1);
            }
			code[i].type = inst_jmp_t;
			code[i].value.jmp = n;
            continue;
		}
		if (code[i].type == inst_for_t || code[i].type == inst_while_t) {
			loop_count++;
			loops = realloc(loops, sizeof(int) * loop_count);
			loops[loop_count - 1] = i;
			continue;
		}
		if (code[i].type == inst_proceed_t) {
			if (code[i].value.jmp_length <= 0) {
				PRINT_ERR("ERROR proceed keyword only accept positive non null integers\n");
				exit(1);
			}
			if (loop_count && code[i].value.jmp_length <= loop_count) {
				code[i].type = inst_jmp_t;
				int idx = loops[loop_count - code[i].value.jmp_length];
				if (code[idx].type == inst_while_t) {
					code[i].value.jmp = code[idx].value.wh->endwhile;
				}
				if (code[idx].type == inst_for_t) {
					code[i].value.jmp = code[idx].value.fo->endfor;
				}
				continue;
			}
			else {
				code[i].type = inst_jmp_t;
				code[i].value.jmp = 0;
				continue;
			}
		}
		if (code[i].type == inst_stop_t) {
			if (code[i].value.jmp_length <= 0) {
				PRINT_ERR("ERROR stop keyword only accept positive non null integers\n");
				exit(1);
			}
			if (loop_count && code[i].value.jmp_length <= loop_count) {
				code[i].type = inst_jmp_t;
				int idx = loops[loop_count - code[i].value.jmp_length];
				if (code[idx].type == inst_while_t) {
					code[i].value.jmp = code[idx].value.wh->endwhile + 1;
				}
				if (code[idx].type == inst_for_t) {
					code[i].value.jmp = code[idx].value.fo->endfor + 1;
				}
				continue;
			}
			else {
				code[i].type = inst_jmp_t;
				code[i].value.jmp = len;
				continue;
			}
		}
		if (inst_endfor_t == code[i].type || inst_endwhile_t == code[i].type) {
			loop_count--;
			if (!loop_count) {
				free(loops);
				loops = NULL;
			}
			else
				loops = realloc(loops, sizeof(int) * loop_count);
			continue;
		}
	}

	return code;
}