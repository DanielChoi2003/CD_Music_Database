#ifndef MCDSCR_H_
#define MCDSCR_H_

#include <ncurses.h>

/*
 * initialize the screen.
 * Return 0 on success or -1 on failure
 */
int init_scrn(void);

/*
 * Close the screen
 */
void close_scrn(void);

/*
 * Redraw the main screen.
 */
void draw_scrn(void);

/*
 * Print the message to the status line
 */
void status(char *msg);

void prep_win(WINDOW *win);

void show_win(WINDOW *win);

void add(char *kbuf, char *vbuf);

void del(char *kbuf);

void find(char *kbuf);

void show(char *kbuf, char *vbuf);

void get(char *kbuf);

void list(char *kbuf, char *vbuf, int cnt);
#endif