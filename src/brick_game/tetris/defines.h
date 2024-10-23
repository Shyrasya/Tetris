#ifndef DEFINES_H
#define DEFINES_H

#define GAME_INIT()       \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    cbreak();             \
    keypad(stdscr, TRUE); \
  }

#define MAP_ROWS 24
#define MAP_COLS 10
#define MAP_START 0
#define QUADR 4
#define TETR 3
#define PADDING 1
#define PLUG 4

#define HORIZ_I_1 6
#define VERTIC_I_2 7
#define VERTIC_I_4 8

#define HUD_WIDTH 8
#define HUD_HEIGHT 20

#define ESC_TO_EXIT "Press \"ESC\" to exit"
#define NO_HIGH_SCORE \
  "Can\'t create/read high_score file. Press any button to exit"
#define NO_HIGH_SCORE_WRITE "Can\'t write high_score file"
#define INTRO_MESSAGE_LEN 21

#define LINE1_SCORE 100
#define LINE2_SCORE 300
#define LINE3_SCORE 700
#define LINE4_SCORE 1500

#define SEC 0.7
#define SEC_DEC 0.07
#define CONV_NSEC 1e-9

#define SUCCESS 0
#define ERROR 1

#define GET_CLICK getch()

#define ESCAPE_BTN 27
#define ENTER_KEY 10
#define PAUSE_KEY 112
#define SPACE_KEY 32

#define MVPRINTW(y, x, ...) mvprintw(y, x, __VA_ARGS__);
#define MVADDCH(y, x, c) mvaddch(y, x, c);

#endif