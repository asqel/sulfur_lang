#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//check if a char is in a string
//*x pointer to the string  |  v char to search
int str_contains_char(char *x,char v);

//check if a string is in a list of string
//list:the list | v:the string to searche | list_len:length of the list
int str_ar_contains_str(char*list[],char*v,int list_len);

int str_ar_contains_char(char*list[],char v,int list_len);

//return the number of occurrence of a char in a string
//*x the string   ||  v the char to count
int str_count(char*x,char v);

//turn a path with \ into a path with /
void back_slash_to_path(char*v);
char*dirname(char*v);
//return new str containing s1+s2
char*str_cat_new(char*s1,char*s2);

char*read_file(char*path);

//return the max power of 10 that can fit into a number
//for number greater or equal than 1 
//ex:
//    1000->3 ; 76787766->7 bc 10^7<76787766<10^8
int get_power10(long double x);

int get_nbr_of_digits(long long int x);

#endif