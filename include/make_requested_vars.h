#ifndef MAKE_REQUESTED_VARS_H
#define MAKE_REQUESTED_VARS_H

#include "../include/make_requested_vars.h"
#include "../include/sulfur.h"
#include "../include/instruction.h"
#include "../include/Ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int add_requested_var(char *var);

Instruction *make_requested_vars(Instruction *inst, int len);


void make_req_vars_inst(Instruction inst);

void make_req_vars_ast(Ast *x);

#endif