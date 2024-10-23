#include "tetris_frontend.h"

void print_borders() {
  print_rectangle(0, MAP_ROWS + PADDING - PLUG, 0, MAP_COLS + PADDING);
  print_rectangle(0, MAP_ROWS + PADDING - PLUG, MAP_COLS + 2,
                  MAP_COLS + HUD_WIDTH + 3);
  MVPRINTW(1, MAP_COLS + 5, "HIGH");
  MVPRINTW(2, MAP_COLS + 5, "SCORE");
  MVPRINTW(5, MAP_COLS + 5, "LEVEL");
  MVPRINTW(10, MAP_COLS + 5, "NEXT");
  MVPRINTW(18, MAP_COLS + 5, "SCORE");
  MVPRINTW(MAP_ROWS + PADDING * 2 - PLUG, PADDING, ESC_TO_EXIT);
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);
  int i = left_x + PADDING;
  for (; i < right_x; i++) {
    MVADDCH(top_y, i, ACS_HLINE);
  }
  MVADDCH(top_y, i, ACS_URCORNER);
  for (int i = top_y + PADDING; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }
  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + PADDING;
  for (; i < right_x; i++) {
    MVADDCH(bottom_y, i, ACS_HLINE);
  }
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_start() {
  MVPRINTW((MAP_COLS / 2) + 2, (MAP_ROWS - INTRO_MESSAGE_LEN) / 2 + 3, "Press");
  MVPRINTW((MAP_COLS / 2) + 3, (MAP_ROWS - INTRO_MESSAGE_LEN) / 2 + 2,
           "\"ENTER\"");
  MVPRINTW((MAP_COLS / 2) + 4, (MAP_ROWS - INTRO_MESSAGE_LEN) / 2 + 5, "to");
  MVPRINTW((MAP_COLS / 2) + 5, (MAP_ROWS - INTRO_MESSAGE_LEN) / 2 + 3, "START");
}

void clear_field() {
  for (int r = 0; r < MAP_ROWS - QUADR; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      MVADDCH(r + PADDING, c + PADDING, ' ');
    }
  }
}

void print_no_high_score() { MVPRINTW(MAP_ROWS + TETR, 1, NO_HIGH_SCORE); }

void clear_score_level() {
  MVPRINTW(3, MAP_COLS + 5, "      ");
  MVPRINTW(6, MAP_COLS + 7, " ");
  MVPRINTW(19, MAP_COLS + 5, "      ");
}

void draw_field(GameInfo_t *statist) {
  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      if (r >= 4 && statist->field[r][c] == 0) {
        MVADDCH(r + PADDING - PLUG, c + PADDING, ' ');
      } else if (r >= 4 &&
                 (statist->field[r][c] == 1 || statist->field[r][c] == 2))
        MVADDCH(r + PADDING - PLUG, c + PADDING, '0');
    }
  }
  MVPRINTW(3, MAP_COLS + 5, "%d", statist->high_score);
  MVPRINTW(6, MAP_COLS + 7, "%d", statist->level);
  MVPRINTW(19, MAP_COLS + 5, "%d", statist->score);
}

void print_next(GameInfo_t statist) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      if (statist.next[r][c] == 1) {
        MVADDCH(10 + r, MAP_COLS + 5 + c, '0');
      }
    }
  }
}

void clear_next() {
  for (int r = 1; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      MVADDCH(10 + r, MAP_COLS + 5 + c, ' ');
    }
  }
}

void print_high_score(GameInfo_t statist) {
  MVPRINTW(3, MAP_COLS + 5, "%d", statist.high_score);
}

void print_game_over() {
  MVPRINTW(2, 4, "GAME");
  MVPRINTW(3, 4, "OVER");
}

void print_pause() { MVPRINTW(8, MAP_COLS + 5, "PAUSE"); }

void clear_pause() { MVPRINTW(8, MAP_COLS + 5, "     "); }
