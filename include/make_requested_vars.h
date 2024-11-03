#ifndef MAKE_REQUESTED_VARS_H
#define MAKE_REQUESTED_VARS_H

#include "make_requested_vars.h"
#include "sulfur.h"
#include "instruction.h"
#include "Ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int add_string_constant(char *str);

int add_requested_var(char *var);

Instruction *make_requested_vars(Instruction *inst, int len);


void make_req_vars_inst(Instruction inst);

void make_req_vars_ast(Ast *x);

int get_requested_var(char *var);

#endif