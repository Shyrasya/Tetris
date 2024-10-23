#define _POSIX_C_SOURCE 200809L
#include "tetris.h"

static struct timespec last_time, cur_time;

UserAction_t get_input(int input) {
  UserAction_t button = Nosig;
  if (input == KEY_UP)
    button = Up;
  else if (input == KEY_DOWN)
    button = Down;
  else if (input == KEY_LEFT)
    button = Left;
  else if (input == KEY_RIGHT)
    button = Right;
  else if (input == SPACE_KEY)
    button = Action;
  else if (input == PAUSE_KEY)
    button = Pause;
  else if (input == ENTER_KEY)
    button = Start;
  else if (input == ESCAPE_BTN)
    button = Terminate;
  return button;
}

void userInput(UserAction_t action, bool *hold) {
  if (action == Down)
    *hold = TRUE;
  else
    *hold = FALSE;
}

void fsm_act(UserAction_t *action, FsmState_t *fsm_state, GameInfo_t *statist,
             PlayerPos_t *figure_info, HelpVar_t *help) {
  switch (*fsm_state) {
    case START:
      clock_gettime(CLOCK_MONOTONIC, &last_time);
      fsm_start(fsm_state);
      break;
    case SPAWN:
      fsm_spawn(fsm_state, statist, figure_info, help);
      break;
    case MOVING:
      fsm_moving(action, fsm_state, figure_info, statist, help);
      break;
    case SHIFTING:
      fsm_shifting(fsm_state, figure_info, statist, help);
      break;
    case ATTACHING:
      fsm_attaching(fsm_state, figure_info, statist);
      break;
    case PAUSE:
      fsm_pause(fsm_state);
      break;
    case GAMEOVER:
      fsm_gameover(fsm_state);
      break;
    case EXIT_STATE:
      break;
  }
}

void fsm_start(FsmState_t *fsm_state) {
  bool code = TRUE;
  while (code) {
    int action = get_input(GET_CLICK);
    switch (action) {
      case Terminate:
        *fsm_state = EXIT_STATE;
        code = FALSE;
        break;
      case Start:
        nodelay(stdscr, TRUE);
        *fsm_state = SPAWN;
        code = FALSE;
        break;
      default:
        break;
    }
  }
}

void fsm_spawn(FsmState_t *fsm_state, GameInfo_t *statist,
               PlayerPos_t *figure_info, HelpVar_t *help) {
  help->turn = 1;
  help->floor = 0;
  figure_info->x = MAP_COLS / 2 - 2;
  figure_info->y = 0;
  figure_info->cur_figure = figure_info->next_figure;
  pattern_fill(statist, figure_info);
  figure_info->next_figure = next_gen(statist);
  cube_to_field(statist, figure_info, help);
  *fsm_state = MOVING;
  clock_gettime(CLOCK_MONOTONIC, &last_time);
}

void fsm_moving(UserAction_t *action, FsmState_t *fsm_state,
                PlayerPos_t *figure_info, GameInfo_t *statist,
                HelpVar_t *help) {
  clock_gettime(CLOCK_MONOTONIC, &cur_time);
  long double elapsed = (cur_time.tv_sec - last_time.tv_sec) +
                        (cur_time.tv_nsec - last_time.tv_nsec) * CONV_NSEC;

  *action = get_input(GET_CLICK);
  switch (*action) {
    case Terminate:
      *fsm_state = EXIT_STATE;
      break;
    case Pause:
      *fsm_state = PAUSE;
      break;
    case Down:
      fsm_down(fsm_state, figure_info, statist, help);
      break;
    case Left:
      if (figure_info->x > MAP_START - 2 * PADDING) {
        fsm_left_right(action, figure_info, statist, help);
      }
      break;
    case Right:
      if (figure_info->x < MAP_COLS - 2 * PADDING) {
        fsm_left_right(action, figure_info, statist, help);
      }
      break;
    case Action:
      fsm_rotate(figure_info, statist, help);
      break;
    default:
      break;
  }
  double delay = SEC - SEC_DEC * statist->speed + SEC_DEC;
  if (elapsed >= delay) {
    *fsm_state = SHIFTING;
  }
}

void fsm_shifting(FsmState_t *fsm_state, PlayerPos_t *figure_info,
                  GameInfo_t *statist, HelpVar_t *help) {
  fsm_down(fsm_state, figure_info, statist, help);
  if (*fsm_state == MOVING) {
    clock_gettime(CLOCK_MONOTONIC, &last_time);
  }
}

void fsm_left_right(UserAction_t *action, PlayerPos_t *figure_info,
                    GameInfo_t *statist, HelpVar_t *help) {
  clear_cubofield(statist, figure_info);
  if (*action == Right)
    figure_info->x++;
  else if (*action == Left)
    figure_info->x--;
  if (cube_to_field(statist, figure_info, help) == ERROR) {
    clear_cubofield(statist, figure_info);
    if (*action == Right)
      figure_info->x--;
    else if (*action == Left)
      figure_info->x++;
  }
  cube_to_field(statist, figure_info, help);
}

void fsm_down(FsmState_t *fsm_state, PlayerPos_t *figure_info,
              GameInfo_t *statist, HelpVar_t *help) {
  if (figure_info->y < MAP_ROWS - QUADR + PADDING) {
    help->down_click = 1;
    clear_cubofield(statist, figure_info);
    figure_info->y++;
    if (figure_info->y + QUADR - 1 > MAP_ROWS ||
        cube_to_field(statist, figure_info, help) == ERROR) {
      help->floor = 1;
      *fsm_state = ATTACHING;
      clear_cubofield(statist, figure_info);
      figure_info->y--;
    } else
      *fsm_state = MOVING;
    cube_to_field(statist, figure_info, help);
    help->down_click = 0;
  } else
    *fsm_state = ATTACHING;
}

void fsm_pause(FsmState_t *fsm_state) {
  nodelay(stdscr, FALSE);
  bool code = TRUE;
  while (code) {
    int action = get_input(GET_CLICK);
    switch (action) {
      case Terminate:
        *fsm_state = EXIT_STATE;
        code = FALSE;
        break;
      case Pause:
        *fsm_state = MOVING;
        nodelay(stdscr, TRUE);
        code = FALSE;
        clock_gettime(CLOCK_MONOTONIC, &last_time);
        break;
      default:
        break;
    }
  }
}

void fsm_rotate(PlayerPos_t *figure_info, GameInfo_t *statist,
                HelpVar_t *help) {
  int copy_cubomatrix[QUADR][QUADR] = {0};
  copy_cube_to_temp(copy_cubomatrix, figure_info);
  clear_cubofield(statist, figure_info);
  switch (figure_info->cur_figure) {
    case J:
    case L:
    case T:
    case S:
    case Z:
      matrix_clockwise(figure_info);
      break;
    case I:
      line_clockwise(statist, figure_info, help);
      break;
    case O:
      break;
  }

  if (cube_to_field(statist, figure_info, help) == ERROR) {
    error_rotate(statist, figure_info, help);
    clean_carry_cube(figure_info);
    copy_temp_to_cube(figure_info, copy_cubomatrix);
  }
  cube_to_field(statist, figure_info, help);
}

void fsm_attaching(FsmState_t *fsm_state, PlayerPos_t *figure_info,
                   GameInfo_t *statist) {
  int fin = check_attach(statist, figure_info);
  if (fin) {
    *fsm_state = GAMEOVER;
    figure_info->next_figure = next_gen(statist);
  } else
    *fsm_state = SPAWN;
}

void fsm_gameover(FsmState_t *fsm_state) {
  nodelay(stdscr, FALSE);
  bool code = TRUE;
  while (code) {
    int action = get_input(GET_CLICK);
    switch (action) {
      case Terminate:
        *fsm_state = EXIT_STATE;
        code = FALSE;
        break;
      case Start:
        nodelay(stdscr, TRUE);
        *fsm_state = SPAWN;
        code = FALSE;
        break;
      default:
        break;
    }
  }
}