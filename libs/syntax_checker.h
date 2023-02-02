#ifndef SYNTAX_CHECKER_H
#define SYNTAX_CHECKER_H
#include "token_class.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
0:no error
1:unknow error
2:missing semicolon
3:missing closing parentheses
4:missing closing brackets
5:missing closing round brackets
6:missing opening parentheses
7:missing opening brackets
8:missing opening round brackets
9:intersection of parentheses or brackets : [{]} / ([)] 
10:missing parentheses after for
11:missing round brackets after for condition
12:missing parentheses after while
13:missing round brackets after while condition
14:missing parentheses after while
15:missing round brackets after while condition
16:nothing between two comma : [3,4,,6]
17:missing operand for + on right
18:missing operand for + on both side
19:missing operand for - on right
20:missing operand for - on both side
21:missing operand for * on right
22:missing operand for * on left
23:missing operand for * on both side
24:missing operand for / on right
25:missing operand for / on left
26:missing operand for / on both side
*/

//return [line number,code error]
//if no error return [0,-1]
int*check_syntax(Token*tokens);




#endif

