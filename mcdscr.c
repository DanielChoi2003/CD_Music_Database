#include <ncurses.h>
#include <form.h>
#include "mcdscr.h"

WINDOW *mainwin;   // Main window
WINDOW *menuwin;   // menu bar at the top of main window
WINDOW *statuswin; // Status line at the bottom of main window
WINDOW *workwin;   // input/output area of main window

int init_scrn(void)
{
    int maxy, maxx;

    // returns stdscr on success
    if ((mainwin = initscr()) == NULL)
    {
        perror("mcdscr.h: screen initialization");
        return -1;
    }

    getmaxyx(stdscr, maxy, maxx);

    // create a subwindow for menu
    if ((menuwin = derwin(stdscr, 1, maxx, 0, 0)) == NULL)
    {
        perror("mcdscr.c: menu screen initialization");
        return -1;
    }

    // create a subwindow for status messages
    if ((statuswin = derwin(stdscr, 1, maxx, maxy - 1, 0)) == NULL)
    {
        perror("mcdscr.c: status screen initialization");
        return -1;
    }

    // create a subwindow for input and output
    if ((workwin = derwin(stdscr, maxy - 2, maxx, 1, 0)) == NULL)
    {
        perror("mcdscr.c: input/output window initialization");
        return -1;
    }

    // information entered immediately without ENTER
    if (cbreak() == ERR)
    {
        return -1;
    }

    if (keypad(stdscr, TRUE) == ERR) // to allow F keys
    {
        return -1;
    }

    if (noecho() == ERR) // doesn't repeat what was inputted
    {
        return -1;
    }

    return 0;
}

void close_scrn(void)
{
    // nocbreak(); <- unneeded?
    /*
     * free the subwindows' memory.
     * endwin() simply invalidates them. Doesn't free.
     */
    delwin(menuwin);
    delwin(statuswin);
    delwin(workwin);
    endwin();
}

void draw_scrn(void)
{

    char *menu = "F2-Add    F3-Delete      F4-Find      F5-Get      F6-List     F10-Quit";

    /*   prints a string on (0, 0) in menu subwindow   */
    mvwprintw(menuwin, 0, 0, "%s", menu);
    wrefresh(menuwin);
    refresh();
}

void status(char *msg)
{

    werase(statuswin);
    mvwprintw(statuswin, 0, 0, "%s", msg);
    wrefresh(statuswin);
    refresh();
}

void prep_win(WINDOW *window)
{
    werase(window);
    echo();
}

void show_win(WINDOW *window)
{
    noecho();
    wrefresh(window);
    refresh();
}

void add(char *kbuf, char *vbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST: ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    mvwprintw(workwin, 2, 0, "TITLE: ");
    status("Enter value");
    wgetstr(workwin, vbuf);
    show_win(workwin);
}

void del(char *kbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST: ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    show_win(workwin);
}

void find(char *kbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST: ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    show_win(workwin);
}

void show(char *kbuf, char *vbuf)
{
    werase(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST: %s", kbuf);
    mvwprintw(workwin, 2, 0, "TITLE: %s", vbuf);
    show_win(workwin);
}

void list(char *kbuf, char *vbuf, int cnt)
{
    int maxx, maxy, nexty;

    getmaxyx(workwin, maxy, maxx);

    /*
     * if there has been no record printed yet,
     * then start with a header
     */
    if (cnt == 0)
    {
        werase(workwin);
        mvwhline(workwin, cnt, 0, ACS_HLINE, maxx); // puts a horizontal line across the screen
        mvwprintw(workwin, cnt + 1, 0, "ARTIST");
        mvwprintw(workwin, cnt + 1, maxx / 2, "TITLE");
        mvwhline(workwin, cnt + 2, 0, ACS_HLINE, maxx);
    }

    nexty = cnt + 3;
    mvwprintw(workwin, nexty, 0, "%s", kbuf);
    mvwprintw(workwin, nexty, maxx / 2, "%s", vbuf);
    show_win(workwin);
}