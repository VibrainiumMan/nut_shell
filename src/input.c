#include "input.h"
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*enum Keys {
	UP_KEY = 1000,
	DOWN_KEY = 1001,
	LEFT_KEY = 1002,
	RIGHT_Key = 1003
};

typedef struct {
	float x = 0.0f;
	float y = 0.0f;
}M_Curs;*/


struct termios o_term;

void raw_mode() {
	if(tcgetattr(STDIN_FILENO, &o_term) < 0) {
		exit(1);
	}
	atexit(canonical_m);
	
	struct termios raw = o_term; 
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
		exit(1);
	}
}

void canonical_m() {
	if(tcsetattr(STDIN_FILENO, TCSANOW, &o_term)) {
		exit(1);
	}
}

/*int read_key() {
	int read;
	char c;

	return c;
}

void move_cursor(int k) {
	M_Curs m;
	switch(k) {
		case LEFT_KEY:
			m.x--;
			break;
		case RIGHT_KEY:
			m.x++;
			break;
	}
}*/
