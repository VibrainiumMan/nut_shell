#ifndef __BUILTINS__
#define __BUILTINS__

#include <readline/readline.h>
#include <readline/history.h>

extern char *builtin_s[];
extern int (*builtin_f[]) (char**);

int redirect_file(char **args);
int exec_builtin(char **args);
int b_cd(char **args);
int b_exit(char **args);
int b_help(char **args);

int b_history(char **args);

int builtin_length();
#endif
