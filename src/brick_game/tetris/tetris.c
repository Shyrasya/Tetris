#include "tetris.h"

int main() {
  GAME_INIT();
  setlocale(LC_ALL, "");
  game_cycle();
  endwin();
  return SUCCESS;
}

void game_cycle() {
  FsmState_t fsm_state = START;
  PlayerPos_t figure_info;
  GameInfo_t statist = updateCurrentState(&fsm_state);
  HelpVar_t help;
  UserAction_t action;
  bool hold = FALSE;
  bool break_flag = TRUE;
  srand(time(NULL));

  if (statist.next != NULL) {
    first_fig_init(&statist, &figure_info, &help);
  }
  while (break_flag) {
    statist = updateCurrentState(&fsm_state);
    fsm_act(&action, &fsm_state, &statist, &figure_info, &help);
    userInput(action, &hold);
    while (hold && fsm_state == MOVING) {
      fsm_down(&fsm_state, &figure_info, &statist, &help);
      statist = updateCurrentState(&fsm_state);
      action = get_input(GET_CLICK);
    }
    if (fsm_state == EXIT_STATE) break_flag = FALSE;
  }
  write_high_score(&statist);
  statist_close(&statist);
}

GameInfo_t updateCurrentState(FsmState_t *fsm_state) {
  static GameInfo_t statist;
  if (*fsm_state == PAUSE) {
    print_pause();
    statist.pause = 1;
  } else
    statist.pause = 0;
  if (*fsm_state == START) {
    start_upd_cur_st(&statist, fsm_state);
  } else if (*fsm_state == GAMEOVER) {
    gameover_upd_cur_st(&statist);
  } else if (*fsm_state == SPAWN) {
    check_full_lines(&statist);
    clear_field();
    clear_next();
    clear_score_level();
    draw_field(&statist);
  } else if (*fsm_state == MOVING) {
    print_next(statist);
    clear_pause();
    draw_field(&statist);
  } else if (*fsm_state == ATTACHING || *fsm_state == SHIFTING) {
    draw_field(&statist);
  }
  refresh();
  return statist;
}

void start_upd_cur_st(GameInfo_t *statist, FsmState_t *fsm_state) {
  print_borders();
  print_start();
  if (statist->field == NULL && statist->field == NULL) {
    int init = statist_init(statist);
    if (init == ERROR) {
      *fsm_state = EXIT_STATE;
      clear_field();
      print_no_high_score();
      nodelay(stdscr, FALSE);
      refresh();
      get_input(GET_CLICK);
    } else
      print_high_score(*statist);
  }
}

void gameover_upd_cur_st(GameInfo_t *statist) {
  clear_gameovr_field(statist);
  int write_hs = write_high_score(statist);
  if (write_hs == ERROR) print_no_high_score();
  statist->level = 1;
  statist->score = 0;
  statist->speed = 1;
  clear_field();
  clear_next();
  print_game_over();
  print_start();
  print_high_score(*statist);
}
