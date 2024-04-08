#include <stdlib.h>
#include <string.h>

#include "../include/preprocessor.h"
#include "../include/utilities.h"

int is_there_brack_elif(int from, char *input) {
    int i = from + 1;
    if (input[from] == '\0')
        return 0;
    while (input[i] == ' ' || input[i] == '\n' || input[i] == '\t' || input[i] == '\r') {
        i++;
    }
    if(input[i] == '{' || input[i] == '(')
        return 1;
    if(input[i] == 'e' && input[i + 1] == 'l' && input[i + 2] == 'i' && input[i + 3] == 'f')
        return 1;
    if(input[i] == 'e' && input[i + 1] == 'l' && input[i + 2] == 's' && input[i + 3] == 'e')
        return 1;
    return 0;

}

char *add_semicolon(char *input) {
    char *text = malloc(sizeof(char) * (strlen(input) + 2 + str_count(input, '\n')));
    int in_string = 0;
    int open_parenthesis = 0;

    int t_index = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '"' && (in_string && input[i - 1] != '\\')) {
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
        
        if (open_parenthesis || input[i] == ';' || input[i] == '{' || input[i] == '}' || input[i] == '\n')
            continue;

        if ((input[i+1] == '\n' || input[i+1] == '\0') && !is_there_brack_elif(i + 1, input))
            text[t_index++] = ';';
        
        else if (input[i+1] == '/' && input[i+2] == '/')
            text[t_index++] = ';';
    }
    text[t_index] = '\0';
    text = realloc(text, t_index + 1000);

    return text;
}

char *preprocess(char *input) {
    char *output = add_semicolon(input);
    return output;
}
