#include "../include/make_include.h"
#include "../include/token_class.h"
#include "../include/sulfur.h"
#include "../include/utilities.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/get_include_tokens.h"


char **included_files = NULL;
int	included_files_len = 0;

void reset_included_files() {
	for (int i = 0; i < included_files_len; i++) {
		free(included_files[i]);
	}
	free(included_files);
	included_files = NULL;
	included_files_len = 0;
}


/*
!TODO:
	include path
	include path else ()
	include path mode
	include path mode else ()
	cahnge include:
		include PATH MODE else (VALUE)

		PATH : path to file to include (str token)
			files that starts with "//" are in the foolder s_include in the folder of the interpretery
				"//test.su" if sulfur is in $HOME/sulfur/ : $HOME/sulfur/test.su

			relatives files are relative to the folder of the files which the include is in

		MODE : (identifier token)(optional)
			once : only included the file if hasn't been included already
			string : creates a string with the content of file (be carefull with binary files containing 0x00)
			number : creates an ount or a floap depending on the content  of the file if the parsing of the number fail
					the VALUE will be used if there is no  else the value nill will be used
			floap : same as number but only for floap (if the file doesn't contain '.' it will also be a floap)
			ount : same as number but only for ount
			binary_list : ascii vals in list
		VALUE:
			is used if there is an error during the include if not specified may throw an error
			

		example:
			include "secret_number.txt" ount else (0)
			include "//print.su" once else ()
			include "../other_folder/script.su"
			lorem_ipsum = include "lorem_ipsum.txt" string else ("")



*/

char *evaluate_include_path(char *path, char *dir_path) {
	char *res;
	if (path[0] == '/' && path[1] ==  '/')
		return str_cat_new3(S_INCLUDE_DIR, "/", &(path[2]));
	else if (uti_is_path_relative(path))
		return str_cat_new3(dir_path, "/", path);
	else
		return uti_strdup(path);
}


Token *make_include(Token *toks, int *len, char *path_arg){
	int p = 0;
	char *dir_path;
	if (!strcmp(path_arg, "*shell*")) {
		dir_path = uti_strdup(INTERPRETER_DIR);
	}
	else {
		dir_path = uti_dirname(path_arg);
	}
	while (p < *len){
		if (toks[p].type == keyword && *toks[p].value.t == include_t) {
			if (p + 1 < *len && toks[p + 1].type == str) {
				char *new_path = evaluate_include_path(toks[p + 1].value.s, dir_path);
				char *mod = NULL;
				int else_start = -1;
				int else_end = -1;
				if (p + 2 < *len && toks[p + 2].type == identifier) {
					//include path mode else (...)
					if (p + 3 < *len && toks[p + 3].type == keyword && *toks[p + 3].value.t == else_t) {
						if (!(p + 4 < *len && toks[p + 4].type == syntax && *toks[p + 4].value.t == par_L)) {
							PRINT_ERR("ERROR on include keyword missing '(' after else\n\ttry include '%s' %s else (...)\n", new_path, toks[p + 2].value.s);
							exit(1);
						}
						else_start = p + 4;
						else_end = search_rpar(toks, else_start);
						if (else_end == -1) {
							PRINT_ERR("ERROR on include keyword missing closing ')' after else\n\ttry include '%s' %s else (...)\n", new_path, toks[p + 2].value.s);
							exit(1);
						}
						int include_tokens_len = 0;
						//include tokens == NULL : ERROR, error is stored in include_tokens_len 0 if we can use the else
						Token *include_tokens = get_include_tokens(&include_tokens_len, toks[p + 2].value.s, new_path);
						if (include_tokens == NULL && include_tokens_len != 0) {
							PRINT_ERR("ERROR on include file '%s' with mod '%s'\n", new_path, toks[p + 2].value.s);
							exit(1);
						}
						free(new_path);
						if (include_tokens == NULL) {
							// p-1    p    p+1  p+2  p+3 p+4
							// ... include path mode else (...) ...
							free(toks[p].value.t);
							free(toks[p + 1].value.s);
							free(toks[p + 2].value.s);
							free(toks[p + 3].value.t);
							free(toks[p + 4].value.t);
							free(toks[else_end].value.t);
							for(int i = else_end; i < *len - 1; i++) {
								toks[i] = toks[i + 1];
							}
							(*len)--;
							toks = realloc(toks, sizeof(Token) * (*len + 1));
							toks[*len] = end_token;
							for(int i = else_start + 1; i < *len; i++) {
								toks[i - 5] = toks[i];
							}
							(*len) -= 5;
							toks = realloc(toks, sizeof(Token) * (*len + 1));
							toks[*len] = end_token;
							continue;
						}
						else {
							// p-1    p    p+1  p+2  p+3 p+4
							// ... include path mode else (...) ...
							Token *new_toks = malloc(sizeof(Token) * (*len - (else_end - p + 1) + include_tokens_len + 1));
							int new_toks_end = 0;
							for(int i = 0; i < p; i++) {
								new_toks[new_toks_end] = toks[i];
								new_toks_end++;
							}
							for(int i = 0; i < include_tokens_len; i++) {
								new_toks[new_toks_end] = include_tokens[i];
								new_toks_end++;
							}
							for(int i = else_end + 1; i < *len; i++) {
								new_toks[new_toks_end] = toks[i];
								new_toks_end++;
							}
							new_toks[new_toks_end] = end_token;
							for(int i = p; i <= else_end; i++) {
								free_tok_val(toks[i]);
							}
							free(toks);
							toks = new_toks;
							*len = *len - (else_end - p + 1) + include_tokens_len;
							free(include_tokens);
							continue;
						}
					}
					else {

					}
				}
				else if (toks[p + 1].type == keyword && *toks[p + 1].value.t == else_t) {

				}
				else {

				}
			}
			//if (p + 1 < *len && toks[p + 1].type == str){
			//	if (p + 2 < *len && toks[p + 2].type == identifier && !strcmp("once", toks[p + 2].value.s)) {
			//		char *path = NULL;
			//		if(toks[p + 1].value.s[0] == '/' && toks[p + 1].value.s[1] == '/'){
			//			path = str_cat_new(dir_path, "/");
			//			char *old_path = path;
			//			path = str_cat_new(path, &toks[p + 1].value.s[2]);
			//			free(old_path);
			//		}
			//		else{
			//			path = strdup(toks[p + 1].value.s);
			//		}
			//		//check if path has already been included
			//		char has_been_included = 0;
			//		for (int i = 0; i < included_files_len; i++) {
			//			if (!strcmp(included_files[i], path)) {
			//				has_been_included = 1;
			//				break;
			//			}
			//		}
			//		if (has_been_included) {
			//			p += 3;
			//			continue;
			//		}
			//		//add file in included files
			//		included_files_len++;
			//		included_files = realloc(included_files, sizeof(char *) * included_files_len);
			//		included_files[included_files_len - 1] = strdup(path);
			//		char *text = read_file(path);
			//		if(!text){
			//			PRINT_ERR("ERROR on include file '%s' doesn't exists\n",path);
			//			exit(1);
			//		}
			//		Token *l = lexe(text);
    		//		int len2 = token_len(l);
    		//		l = make_include(l, &len2, path);
			//		free(path);
			//		Token *new_tok = malloc(sizeof(Token) * ((*len - 3) + len2 + 1));
			//		int k = 0;
			//		for(int i = 0; i < p; i++){
			//			new_tok[k++] = toks[i];
			//		}
			//		for(int i = 0; i < len2; i++){
			//			new_tok[k++] = l[i];
			//		}
			//		for(int i = p + 2; i < *len; i++){
			//			new_tok[k++] = toks[i];
			//		}
			//		new_tok[(*len - 2) + len2] = toks[*len];
			//		free_tok_val(toks[p]);		// free the value of the include keyword
			//		free_tok_val(toks[p + 1]);	// free the value of the include path
			//		free_tok_val(toks[p + 2]);	// free the value of the once identifier
			//		free(toks);
			//		free(l);
			//		free(text);
			//		toks = new_tok;
			//		*len = (*len - 2) + len2;
			//	}
			//	else {
			//		char *path = NULL;
			//		if(toks[p + 1].value.s[0] == '/' && toks[p + 1].value.s[1] == '/'){
			//			path = str_cat_new(dir_path, "/");
			//			char *old_path = path;
			//			path = str_cat_new(path, &toks[p + 1].value.s[2]);
			//			free(old_path);
			//		}
			//		else{
			//			path = strdup(toks[p + 1].value.s);
			//		}
			//		char *text = read_file(path);
			//		if(!text){
			//			PRINT_ERR("ERROR on include file '%s' doesn't exists\n",path);
			//			exit(1);
			//		}
			//		Token *l = lexe(text);
    		//		int len2 = token_len(l);
    		//		l = make_include(l, &len2, path);
			//		free(path);
			//		Token *new_tok = malloc(sizeof(Token) * ((*len - 2) + len2 + 1));
			//		int k = 0;
			//		for(int i = 0; i < p; i++){
			//			new_tok[k++] = toks[i];
			//		}
			//		for(int i = 0; i < len2; i++){
			//			new_tok[k++] = l[i];
			//		}
			//		for(int i = p + 2; i < *len; i++){
			//			new_tok[k++] = toks[i];
			//		}
			//		new_tok[(*len - 2) + len2] = toks[*len];
			//		free_tok_val(toks[p]);		// free the value of the include keyword
			//		free_tok_val(toks[p + 1]);	// free the value of the include path
			//		free(toks);
			//		free(l);
			//		free(text);
			//		toks = new_tok;
			//		*len = (*len - 2) + len2;
			//	}
			//}
			//else{
			//	toks[p].type = identifier;
			//	free(toks[p].value.t);
			//	toks[p].value.s = strdup("include");
			//	p++;
			//	continue;
			//}
		}
		else{
			p++;
		}
	}
	free(dir_path);
	return toks;
}