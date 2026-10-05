#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "builtins.h"
#include "input.h"

#define USER getenv("USER")
#define HOST getenv("HOSTNAME")

#define LINE_SIZE 1024
#define TOK_BUF 64 
#define DELIM " \" \' \t \r \n" 

char *get_cur_dir();
int s_exec(char **args);
char **parse(char *input);
int redirect_file(char **input);

int main(void) {
	//raw_mode();
	char cwd[PATH_MAX];

	char cmd[LINE_SIZE];
	char *cbuf = malloc(sizeof(cmd));

	int running = 1;
	while(running) {
		printf("%s@%s:%s$ ", USER, HOST, getcwd(cwd, sizeof(cwd)));

		if(fgets(cbuf, sizeof(cmd), stdin) == 0) break;  
		cbuf[strcspn(cbuf, "\n")] = '\0'; 
		if(cbuf[0] == '\0') continue;	
		
		char **args = parse(cbuf);	
		if(exec_builtin(args) < 0) {
			break;
		}
		if(cbuf == NULL) continue; 

		free(args);

	}
	free(cbuf);

	return 0;
}

int s_exec(char **args) {
	pid_t pid = fork();
	int status;

	if(pid == 0) {
		if(redirect_file(args) == -1) {
			exit(1);
		}	
			printf("nsh");

		if(execvp(args[0], args) == -1) {
			fprintf(stderr, "nsh: command not found\n");
			exit(1);
		}
	} else if(pid < 0) {
		perror("nsh");
		exit(1);
	} else {
		do {
			waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

char **parse(char *input) {
	int buf = TOK_BUF;
	int pos = 0;
	char *token;
	char **tokens = malloc(buf * sizeof(char*));
	

	if(tokens == NULL) {
		fprintf(stderr, "Error: Failed to allocate tokens. \n");
		exit(1);
	}

	token = strtok(input, DELIM); 
	while(token != NULL) {
		tokens[pos] = token;
		pos++;
		
		if(pos >= buf) {
			buf += TOK_BUF;
			tokens = realloc(tokens, buf * sizeof(char*));

			if(tokens == NULL) {
				fprintf(stderr, "Error: Failed to reallocate tokens \n");
				exit(1);
			}
		}
		token = strtok(NULL, DELIM);
	}
	
	tokens[pos] = NULL;
	return tokens;
}

int exec_builtin(char **args) {
	if(args[0] == NULL) {
		return 1;
	}

	for(int i = 0; i < builtin_length(); i++) {
		if(strcmp(args[0], builtin_s[i]) == 0) {
			return (*builtin_f[i])(args);
		}
	}
	return s_exec(args);
}
