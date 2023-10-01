#ifndef MAKE_INCLUDE_H
#define MAKE_INCLUDE_H

#include "sulfur.h"
#include "token_class.h"

Token *make_include(Token *toks, int *len, sulfur_args_t *args);

#endif