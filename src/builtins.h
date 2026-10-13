#ifndef __BUILTINS__
#define __BUILTINS__

#define MAX_HIST 100

extern char *builtin_s[];
extern int (*builtin_f[]) (char**);

typedef struct History_buf{
	int hist_count;
        char *history[MAX_HIST];
}History_buf;

int b_exec_pysh(char **args);

int redirect_file(char **args);
int exec_builtin(char **args);
int b_cd(char **args);
int b_exit(char **args);
int b_help(char **args);

int m_save_cmd(char **args);
void free_hist();
int b_history(char **args);

int builtin_length();
#endif
