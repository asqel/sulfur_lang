#include "../include/make_include.h"
#include "../include/token_class.h"
#include "../include/sulfur.h"
#include "../include/utilities.h"
#include "../include/lexer.h"

Token *make_include(Token *toks, int *len, char *path_arg){
	int p = 0;
	char *dir_path = uti_dirname(path_arg);
	while (p < *len){
		if (toks[p].type == keyword && *toks[p].value.t == include_t){
			if (p + 1 < *len && toks[p + 1].type == str){
				char *path = NULL;
				if(toks[p + 1].value.s[0] == '/' && toks[p + 1].value.s[1] == '/'){
					path = str_cat_new(dir_path, "/");
					char *old_path = path;
					path = str_cat_new(path, &toks[p + 1].value.s[2]);
					free(old_path);
				}
				else{
					path = strdup(toks[p + 1].value.s);
				}
				char *text = read_file(path);
				if(!text){
					printf("ERROR on include file '%s' doesn't exists\n",path);
					exit(1);
				}
				Token *l = lexe(text);
    			int len2 = token_len(l);
    			l = make_include(l, &len2, path);
				free(path);
				Token *new_tok = malloc(sizeof(Token) * ((*len - 2) + len2 + 1));
				int k = 0;
				for(int i = 0; i < p; i++){
					new_tok[k++] = toks[i];
				}
				for(int i = 0; i < len2; i++){
					new_tok[k++] = l[i];
				}
				for(int i = p + 2; i < *len; i++){
					new_tok[k++] = toks[i];
				}
				new_tok[(*len - 2) + len2] = toks[*len];
				free(toks);
				free(l);
				free(text);
				toks = new_tok;
				*len = (*len - 2) + len2;
			}
			else{
				toks[p].type = identifier;
				free(toks[p].value.t);
				toks[p].value.s = strdup("include");
				p++;
				continue;
			}
		}
		else{
			p++;
		}
	}
	free(dir_path);
	return toks;
}