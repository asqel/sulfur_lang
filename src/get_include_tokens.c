#include "../include/token_class.h"
#include "../include/get_include_tokens.h"
#include "../include/lexer.h"
#include "../include/utilities.h"


Token *get_include_tokens(int *len, char *mode, char *path) {
	if (!strcmp(mode, "normal"))
		return include_mode_normal(len, mode, path);
	if (!strcmp(mode, "once")) {
		return include_mode_once(len, mode, path);
	}
	else {
		printf("ERROR unkown mode for include path '%s' mode '%s'\n", path, mode);
		exit(1);
	}
}

Token *include_mode_normal(int *len, char *mode, char *path) {
	char *text = read_file(path);
	if (!text) {
		*len = 0;
		return NULL;
	}
	Token *toks = lexe(text);
	*len = token_len(toks);
	free(text);
	return toks; 
}

Token *include_mode_once(int *len, char *mode, char *path) {
	*len=0;
	return NULL;
}