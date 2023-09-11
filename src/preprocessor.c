#include <stdlib.h>
#include <string.h>

#include "../include/preprocessor.h"
#include "../include/utilities.h"

char *add_semicolon(char *input) {
    char *text = malloc(sizeof(char) * (strlen(input) + 2 + str_count(input, '\n')));
    int in_string = 0;
    int open_parenthesis = 0;

    int t_index = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '"') {
            in_string = !in_string;
        }

        if (in_string) {
            text[t_index++] = input[i];
            continue;
        }

        if (input[i] == '(') {
            open_parenthesis++;
        }

        if (input[i] == ')') {
            open_parenthesis--;
        }

        text[t_index++] = input[i];
        if (open_parenthesis == 0 &&
            (input[i] != ';' && input[i] != '{' && input[i] != '}' && input[i] != '\n') &&
            (input[i+1] == '\n' || input[i+1] == '\0')
        ) text[t_index++] = ';';
    }
    text[t_index] = '\0';

    return text;
}

char *preprocess(char *input) {
    return add_semicolon(input);
}
