#ifndef __INPUT__
#define __INPUT__

void raw_mode();
void canonical_m();
void move_cursor(int k);

float cursor_pos(float x);
char read_key();

#endif
