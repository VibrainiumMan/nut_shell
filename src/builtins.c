#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include "builtins.h"

History_buf h_buf;

char *builtin_s[] = {
	"cd",
	"exit",
	"history",
	"help"
};

int (*builtin_f[]) (char**) = {
	&b_cd,
	&b_exit,
	&b_history,
	&b_help
};

int builtin_length() {
	return sizeof(builtin_s) / sizeof(char*);
}

int redirect_file(char **args) {
	int f;

	for(int i = 0; args[i] != NULL; i++) {
		if(strcmp(args[i], ">") == 0) {
			f = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			if(f == -1) {
				perror("nsh");
				return -1;
			}
			
			if(dup2(f, STDOUT_FILENO) < 0) {
				perror("nsh");
				close(f);
				return -1;
			}
			perror("nsh");

			close(f);
			args[i] = NULL;
			return 0;
		} 
		if(strcmp(args[i], ">>") == 0) {
			f = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
			if(f == -1) {
				perror("nsh");
				return -1;
			}

			if(dup2(f, STDOUT_FILENO) < 0) {
				perror("nsh");
				close(f);
				return -1;
			}

			close(f);
			args[i] = NULL; 
			return 0;
		}
		if(strcmp(args[i], "<") == 0) {
			printf("<");
			return 0;
		}
		if(strcmp(args[i], "<<") == 0) {
			printf("<<");
			return 0;
		}
	}
	return 0;
}

int b_cd(char **args) {
	if(args[1] == NULL) {
		char *home = getenv("HOME");

		if(home == NULL) {
			return 1;
		}

		if(chdir(home) != 0) {
			perror("nsh");
		}
	} else {
		if(chdir(args[1]) != 0) {
			perror("nsh");
		}
	}
	return 1;
}

int b_help(char **args) {
	puts("Nut Shell");
	puts("This is the list of the following commands");
	puts("Select a command after \'help.\' i.e \'help history\'");
	if(args[1] == NULL) {
		for(int i = 0; i < builtin_length(); i++) {
			printf(" %d. %s\n", i + 1, builtin_s[i]);
		}
	}
	return 0;
}

int m_save_cmd(char **args) {
        if(args == NULL) {
                return -1;
        } 
        
        char s_cmd[MAX_HIST];
        snprintf(s_cmd, sizeof(s_cmd), "%s", *args);
        s_cmd[strcspn(s_cmd, "\n")] = '\0';
        
        char *l_cmd = strdup(s_cmd);

        h_buf.history[h_buf.hist_count] = l_cmd; 
        h_buf.hist_count++;
	return 1;
}

void free_hist() {
	for(int i = 0; i < h_buf.hist_count; i++) {
                free(h_buf.history[i]);
                h_buf.history[i] = NULL;
        }
        h_buf.hist_count = 0;
}

int b_history(char **args) {
	puts("History");
	if(args[1] == NULL) {
		for(int i; i < h_buf.hist_count; i++) {
                        printf("%s\n", h_buf.history[i]);
		}
	}
	return 1;
}

int b_exit(char **args) {
	int status = 0;

	if(args[1] == NULL) {
		puts("Exiting shell...");
		exit(status);
	}
	
	char *end;
	status = strtol(args[1], &end, 10);
	
	if(status < 0 || status > 256) {
		int m_status = status % 256;
		printf("%d\n", m_status);
		exit(m_status);
	}

	printf("Exiting...\n%d\n", status);
	exit(status);	
} 
