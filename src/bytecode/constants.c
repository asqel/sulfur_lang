#include "../../include/bytecode/constants.h"
#include "../../include/bytecode/bytecode.h"
#include "../../include/instruction.h"
#include "../../include/Ast.h"
#include "../../include/bytecode/bytecode.h"
#include "../../include/memlib.h"


Object *constants = NULL;
S_sulfur_int *constant_adresses = NULL;
S_sulfur_int constants_len = 0;

void reset_constants() {
    if (constants != NULL) {
        for(int i = 0; i < constants_len; i++) {
                Obj_free_val(constants[i]);
        }
        free(constants);
        constants = NULL;
    }
    if (constant_adresses != NULL)
        free(constant_adresses);
    constant_adresses = NULL;
}

S_sulfur_int add_constants(Object x) {
    if (x.type != Obj_ount_t && x.type != Obj_floap_t && x.type != Obj_string_t) {
        printf("ERROR can only place in coosntants  floap ount or string\n");
        exit(1);
    }
    for(int i = 0; i < constants_len; i++) {
        if (x.type == constants[i].type) {
            if (x.type == Obj_string_t && !strcmp(x.val.s, constants[i].val.s)) 
                return i; 
            if (x.type == Obj_floap_t && x.val.f == x.val.f) 
                return i; 
            if (x.type == Obj_floap_t && x.val.i == x.val.i) 
                return i; 
        }
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = Obj_cpy(x);
    return constants_len - 1;
}

S_sulfur_int add_constant_str(char *s) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_string_t && !strcmp(s, constants[i].val.s))
            return i;
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = new_string(s);
    return constants_len - 1;
}


S_sulfur_int add_constant_int(S_ount_t value) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_ount_t && value == constants[i].val.i)
            return i;
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = new_ount(value);
    return constants_len - 1;
}

S_sulfur_int add_constant_float(S_floap_t value) {
    //for(int i = 0; i < constants_len; i++) {
    //    if (constants[i].type == Obj_floap_t && value == constants[i].val.f)
    //        return i;
    //}
    //constants_len++;
    //constants = realloc(constants, sizeof(Object) * constants_len);
    //constants[constants_len - 1] = new_floap(value);
    //return constants_len - 1;
}


S_sulfur_int get_constants(Object x) {
    for(int i = 0; i < constants_len; i++) {
        if (x.type == constants[i].type) {
            if (x.type == Obj_string_t && !strcmp(x.val.s, constants[i].val.s)) 
                return i; 
            if (x.type == Obj_floap_t && x.val.f == x.val.f) 
                return i; 
            if (x.type == Obj_floap_t && x.val.i == x.val.i) 
                return i; 
        }
    }
    printf("ERROR constant not exists\n");
    exit(1);
}

S_sulfur_int get_constant_str(char *s) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_string_t && !strcmp(s, constants[i].val.s))
            return i;
    }
    printf("ERROR constant string not exist\n");
    exit(1);
}

S_sulfur_int get_constant_int(S_ount_t value) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_ount_t && value == constants[i].val.i)
            return i;
    }
    printf("ERROR constant ount not exist\n");
    exit(1);
}

S_sulfur_int get_constant_float(S_floap_t value) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_floap_t && value == constants[i].val.f)
            return i;
    }
    printf("ERROR constant floap not exist\n");
    exit(1);
}

void ast_to_constants(Ast x) {
    if (x.type == Ast_varcall_t)
        add_constant_str(x.root.varcall);
    else if (x.type == Ast_funccall_t) {
        add_constant_str(x.root.fun->name);
        add_constant_int(x.root.fun->nbr_arg);
        for(int i = 0; i < x.root.fun->nbr_arg; i++)
            ast_to_constants(x.root.fun->args[i]);
    }
    else if (x.type == Ast_object_t) {
        if (x.type == Obj_string_t || x.type == Obj_ount_t || x.type == Obj_floap_t) {
            add_constants(*x.root.obj);
        }
    }
    if (x.left)
        ast_to_constants(*x.left);
    if (x.right)
        ast_to_constants(*x.right);
}


void inst_to_constants(Instruction *inst, int len) {
    for(int i = 0; i < len; i++) {
        if (inst[i].type == inst_for_t) {
            add_constant_str(inst[i].value.fo->var_name);
            ast_to_constants(*inst[i].value.fo->start);
            ast_to_constants(*inst[i].value.fo->end);
        }
        else if (inst[i].type == inst_while_t) {
            ast_to_constants(*inst[i].value.wh->condition);
        }
        else if (inst[i].type == inst_if_t) {
            ast_to_constants(*inst[i].value.i->condition);
        }
        else if (inst[i].type == inst_elif_t) {
            ast_to_constants(*inst[i].value.el->condition);
        }
        else if (inst[i].type == inst_expr_t) {
            ast_to_constants(*inst[i].value.expr);
        }
        else if (inst[i].type == inst_return_t) {
            ast_to_constants(*inst[i].value.ret);
        }
        else if (inst[i].type == inst_funcdef_t) {
            add_constant_str(inst[i].value.fc->info.name);
            add_constant_str(inst[i].value.fc->info.description);
            add_constant_int(inst[i].value.fc->args_len);
            for(int i = 0; i < inst[i].value.fc->args_len; i++) {
                add_constant_str(inst[i].value.fc->args[i]);
            }
            inst_to_constants(inst[i].value.fc->code, inst[i].value.fc->code_len);
        }
    }
}

void constants_to_bytecode(Bytecode_t *code) {
    constant_adresses = malloc(sizeof(S_sulfur_int) * constants_len);
    for(int i = 0; i < constants_len; i++) {
        constant_adresses[i] = code->len;
        if (constants[i].type == Obj_string_t)
            bytecode_add_char_arr(code, constants[i].val.s, strlen(constants[i].val.s) + 1);
        else if (constants[i].type == Obj_ount_t)
            bytecode_append_llint(code, constants[i].val.i);
        else if (constants[i].type == Obj_floap_t) {
            code->bytes = realloc(code->bytes, code->len + 8);
            double val = ((double)constants[i].val.f);
            memcpy(&code->bytes[code->len], &val, sizeof(double));
            code->len += 8;
        }
    }
}


int bc_get_const_addr_str(char * str) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_string_t && !strcmp(str, constants[i].val.s)) {
            return constant_adresses[i];
        }
    }
    return 0;
}

int bc_get_const_addr_int(S_sulfur_int i) {
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type == Obj_ount_t && i == constants[i].val.i) {
            return constant_adresses[i];
        }
    }
    return 0;
}

int bc_get_const_addr(Object *x) {
    for(int i = 0; i < constants_len; i++) {
        if (x->type == constants[i].type) {
            if (x->type == Obj_string_t && !strcmp(x->val.s, constants[i].val.s))
                return constant_adresses[i];
            if (x->type == Obj_ount_t && x->val.i == constants[i].val.i)
                return constant_adresses[i];
            if (x->type == Obj_floap_t && x->val.f == constants[i].val.f)
                return constant_adresses[i];
        }
    }
    return 0;
}