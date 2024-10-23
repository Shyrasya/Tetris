#include "tetris_backend.h"

int statist_init(GameInfo_t *statist) {
  int code = SUCCESS;
  statist->field = (int **)calloc(MAP_ROWS, sizeof(int *));
  if (statist->field == NULL) code = ERROR;
  for (int r = 0; r < MAP_ROWS && !code; r++) {
    statist->field[r] = (int *)calloc(MAP_COLS, sizeof(int));
    if (statist->field[r] == NULL) code = ERROR;
  }
  if (code == SUCCESS) {
    statist->next = (int **)calloc(QUADR, sizeof(int *));
    if (statist->next == NULL) code = ERROR;
    for (int r = 0; r < QUADR && !code; r++) {
      statist->next[r] = (int *)calloc(QUADR, sizeof(int));
      if (statist->next[r] == NULL) code = ERROR;
    }
  }
  if (!code) success_init(statist, &code);
  return code;
}

void success_init(GameInfo_t *statist, int *code) {
  int high = SUCCESS;
  int read = read_high_score(&high);
  if (!read) {
    statist->high_score = high;
    statist->score = 0;
    statist->level = 1;
    statist->speed = 1;
    statist->pause = 0;
  } else
    *code = ERROR;
}

void statist_close(GameInfo_t *statist) {
  if (statist->field != NULL) {
    for (int i = 0; i < MAP_ROWS; i++) {
      free(statist->field[i]);
    }
    free(statist->field);
    statist->field = NULL;
  }
  if (statist->next != NULL) {
    for (int i = 0; i < QUADR; i++) {
      free(statist->next[i]);
    }
    free(statist->next);
    statist->next = NULL;
  }
  statist->score = 0;
  statist->high_score = 0;
  statist->level = 0;
  statist->speed = 0;
  statist->pause = 0;
}

int read_high_score(int *high) {
  int code = SUCCESS;
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    if (fscanf(file, "%d", high) == 1)
      fclose(file);
    else {
      code = ERROR;
      fclose(file);
    }
  } else {
    file = fopen("high_score.txt", "w");
    if (file) {
      fprintf(file, "0");
      fclose(file);
    } else
      code = ERROR;
  }
  return code;
}

int write_high_score(GameInfo_t *statist) {
  int code = SUCCESS;
  FILE *file = fopen("high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", statist->high_score);
    fclose(file);
  } else
    code = ERROR;
  return code;
}

void first_fig_init(GameInfo_t *statist, PlayerPos_t *figure_info,
                    HelpVar_t *help) {
  figure_info->cur_figure = next_gen(statist);
  pattern_fill(statist, figure_info);
  figure_info->next_figure = next_gen(statist);
  help->turn = 1;
  help->floor = 0;
  help->down_click = 0;
  cube_to_field(statist, figure_info, help);
}

void pattern_fill(GameInfo_t *statist, PlayerPos_t *figure_info) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      figure_info->cube[r][c] = statist->next[r][c];
    }
  }
}

int cube_to_field(GameInfo_t *statist, PlayerPos_t *figure_info,
                  HelpVar_t *help) {
  int code = SUCCESS;
  for (int r = 0; r < QUADR && code == SUCCESS; r++) {
    for (int c = 0; c < QUADR && code == SUCCESS; c++) {
      code = check_cube_to_field(statist, figure_info, r, c, help);
    }
  }
  return code;
}

int check_cube_to_field(GameInfo_t *statist, PlayerPos_t *figure_info, int r,
                        int c, HelpVar_t *help) {
  int code = SUCCESS;
  int cordy = r + figure_info->y;
  int cordx = c + figure_info->x;
  if (figure_info->cube[r][c] == 1) {
    if ((cordy < MAP_ROWS) && (cordx >= MAP_START) && (cordx < MAP_COLS)) {
      if (statist->field[cordy][cordx] != 2)
        statist->field[cordy][cordx] = figure_info->cube[r][c];
      else {
        code = ERROR;
        if (help->down_click) help->floor = 1;
      }
    } else
      code = ERROR;
  }
  return code;
}

void clear_cubofield(GameInfo_t *statist, PlayerPos_t *figure_info) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      int cordy = r + figure_info->y;
      int cordx = c + figure_info->x;
      if (cordy < MAP_ROWS && cordx < MAP_COLS && cordx >= MAP_START) {
        if (statist->field[cordy][cordx] == 1) statist->field[cordy][cordx] = 0;
      }
    }
  }
}

void clean_carry_cube(PlayerPos_t *figure_info) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      figure_info->cube[r][c] = 0;
    }
  }
}

void matrix_clockwise(PlayerPos_t *figure_info) {
  int copy[TETR][TETR] = {0};
  for (int r = 1; r < TETR + 1; r++) {
    for (int c = 0; c < TETR; c++) {
      copy[r - 1][c] = figure_info->cube[r][c];
    }
  }
  int temp[TETR][TETR] = {0};
  for (int r = 0; r < TETR; r++) {
    for (int c = 0; c < TETR; c++) {
      temp[r][c] = copy[TETR - c - 1][r];
    }
  }
  clean_carry_cube(figure_info);
  for (int r = 0; r < TETR; r++) {
    for (int c = 0; c < TETR; c++) {
      figure_info->cube[r + 1][c] = temp[r][c];
    }
  }
}

void line_clockwise(GameInfo_t *statist, PlayerPos_t *figure_info,
                    HelpVar_t *help) {
  int line_fig = HORIZ_I_1;
  clear_cubofield(statist, figure_info);
  clean_carry_cube(figure_info);
  if (help->turn % 4 == 1) {
    line_fig = VERTIC_I_2;
  } else if (help->turn % 4 == 3) {
    line_fig = VERTIC_I_4;
  }
  clockwise_fill(figure_info, line_fig);
  if (help->turn % 4 == 2)
    figure_info->x++;
  else if (help->turn % 4 == 3)
    figure_info->y++;
  else if (help->turn % 4 == 0) {
    figure_info->x--;
    figure_info->y--;
  }
  help->turn++;
}

void copy_cube_to_temp(int (*temp_cube)[QUADR], PlayerPos_t *figure_info) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      temp_cube[r][c] = figure_info->cube[r][c];
    }
  }
}

void copy_temp_to_cube(PlayerPos_t *figure_info, int (*temp_cube)[QUADR]) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      figure_info->cube[r][c] = temp_cube[r][c];
    }
  }
}

void check_full_lines(GameInfo_t *statist) {
  int rows = 0, r = MAP_ROWS - PADDING, flag;
  for (; r > QUADR - PADDING; r--) {
    flag = 0;
    for (int c = 0; !flag && c < MAP_COLS; c++) {
      if (statist->field[r][c] == 0) flag = 1;
    }
    if (!flag) {
      rows++;
      for (int rm = r; rm > QUADR - PADDING; rm--) {
        for (int cm = 0; cm < MAP_COLS; cm++) {
          statist->field[rm][cm] = statist->field[rm - 1][cm];
        }
      }
      r++;
    }
  }
  new_score_set(statist, rows);
  if (statist->score > statist->high_score) {
    statist->high_score = statist->score;
  }
}

void new_score_set(GameInfo_t *statist, int rows) {
  int lines_score[5] = {0, 100, 300, 700, 1500};
  static int speed_score = 0;
  statist->score += lines_score[rows];
  speed_score += lines_score[rows];
  if (speed_score >= 600) {
    if (statist->level < 10) {
      statist->level++;
      statist->speed++;
      speed_score = 0;
    }
  }
}

void clear_gameovr_field(GameInfo_t *statist) {
  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      statist->field[r][c] = 0;
    }
  }
}

int next_gen(GameInfo_t *statist) {
  int next_fig = rand() % MAX_RAND;
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      statist->next[r][c] = figures[next_fig][r][c];
    }
  }
  return next_fig;
}

void clockwise_fill(PlayerPos_t *figure_info, int line_fig) {
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      figure_info->cube[r][c] = figures[line_fig][r][c];
    }
  }
}

void error_rotate(GameInfo_t *statist, PlayerPos_t *figure_info,
                  HelpVar_t *help) {
  clear_cubofield(statist, figure_info);
  if (figure_info->cur_figure == I) {
    help->turn--;
    if (help->turn % QUADR == 2)
      figure_info->x--;
    else if (help->turn % QUADR == 3)
      figure_info->y--;
    else if (help->turn % QUADR == 0) {
      figure_info->x++;
      figure_info->y++;
    }
  }
}

int check_attach(GameInfo_t *statist, PlayerPos_t *figure_info) {
  int code = SUCCESS;
  for (int r = 0; r < QUADR; r++) {
    for (int c = 0; c < QUADR; c++) {
      int cordy = r + figure_info->y;
      int cordx = c + figure_info->x;
      if (cordy < MAP_ROWS) {
        if (statist->field[cordy][cordx] == 1 && (cordy) > QUADR) {
          statist->field[cordy][cordx] = 2;
        } else if (statist->field[cordy][cordx] == 1 && (cordy) <= QUADR)
          code = ERROR;
      }
    }
  }
  return code;
}
