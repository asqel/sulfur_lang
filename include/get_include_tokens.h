#ifndef GET_INCLUDE_TOKENS_H
#define GET_INCLUDE_TOKENS_H


extern char **included_files;
extern int	included_files_len;

Token *get_include_tokens(int *len, char *mode, char *path);

Token *include_mode_once(int *len, char *mode, char *path);
Token *include_mode_normal(int *len, char *mode, char *path);

#endif