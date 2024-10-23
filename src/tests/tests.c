#define _POSIX_C_SOURCE 200809L
#include <check.h>

#include "../brick_game/tetris/defines.h"
#include "../brick_game/tetris/objects.h"
#include "../brick_game/tetris/tetris.h"
#include "../brick_game/tetris/tetris_backend.h"

START_TEST(test1_read_high_score) {
	int high = 1000;
	read_high_score(&high);
  ck_assert_int_eq(high, 1000);
}
END_TEST

START_TEST(test2_read_high_score) {
	int high = 0;
  remove("high_score.txt");
	read_high_score(&high);
  ck_assert_int_eq(high, 0);
}
END_TEST

START_TEST(test3_write_high_score) {
	GameInfo_t statist;
  int code = SUCCESS;
  statist.high_score = 1000;
	code = write_high_score(&statist);
  ck_assert(code == ERROR || code == SUCCESS);
}
END_TEST

START_TEST(test4_pattern_fill) {
	GameInfo_t statist;
  PlayerPos_t figure_info;
  HelpVar_t help;
  statist_init(&statist);
  figure_info.y = 0;
  figure_info.x = MAP_COLS / 2 - 2;
  help.turn = 1;
  help.floor = 0;
  help.down_click = 0;
  statist.next[2][0] = 1;
  statist.next[2][1] = 1;
  statist.next[3][0] = 1;
  statist.next[3][1] = 1;
  pattern_fill(&statist, &figure_info);
  cube_to_field(&statist, &figure_info, &help);
  ck_assert_int_eq(statist.field[2][3], 1);
  ck_assert_int_eq(statist.field[2][4], 1);
  ck_assert_int_eq(statist.field[3][3], 1);
  ck_assert_int_eq(statist.field[3][4], 1);
  statist_close(&statist);
}
END_TEST

START_TEST(test5_clear_cubofield) {
	GameInfo_t statist;
  PlayerPos_t figure_info;
  statist_init(&statist);
  figure_info.y = 0;
  figure_info.x = MAP_COLS / 2 - 2;
  statist.field[2][3] = 1;
  statist.field[2][4] = 1;
  statist.field[3][3] = 1;
  statist.field[3][4] = 1;
  clear_cubofield(&statist, &figure_info);
  ck_assert_int_eq(statist.field[2][3], 0);
  ck_assert_int_eq(statist.field[2][4], 0);
  ck_assert_int_eq(statist.field[3][3], 0);
  ck_assert_int_eq(statist.field[3][4], 0);
  statist_close(&statist);
}
END_TEST

START_TEST(test6_clean_carry_cube) {
	GameInfo_t statist;
  PlayerPos_t figure_info;
  statist_init(&statist);
  figure_info.y = 0;
  figure_info.x = MAP_COLS / 2 - 2;
  statist.next[2][0] = 1;
  statist.next[2][1] = 1;
  statist.next[3][0] = 1;
  statist.next[3][1] = 1;
  pattern_fill(&statist, &figure_info);
  clean_carry_cube(&figure_info);
  ck_assert_int_eq(statist.field[2][3], 0);
  ck_assert_int_eq(statist.field[2][4], 0);
  ck_assert_int_eq(statist.field[3][3], 0);
  ck_assert_int_eq(statist.field[3][4], 0);

  statist_close(&statist);
}
END_TEST


START_TEST(test7_matrix_clockwise) {
  PlayerPos_t figure_info;
  clockwise_fill(&figure_info, 0);
  matrix_clockwise(&figure_info);
  ck_assert_int_eq(figure_info.cube[1][1], 1);
  ck_assert_int_eq(figure_info.cube[1][2], 1);
  ck_assert_int_eq(figure_info.cube[2][1], 1);
  ck_assert_int_eq(figure_info.cube[3][1], 1);
}
END_TEST

START_TEST(test8_line_clockwise) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  statist_init(&statist);
  help.turn = 1;
  figure_info.y = 0;
  figure_info.x = 0;
  clockwise_fill(&figure_info, 6);
  cube_to_field(&statist, &figure_info, &help);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);

  ck_assert_int_eq(statist.field[0][2], 1);
  ck_assert_int_eq(statist.field[1][2], 1);
  ck_assert_int_eq(statist.field[2][2], 1);
  ck_assert_int_eq(statist.field[3][2], 1);
  statist_close(&statist);
}
END_TEST

START_TEST(test9_line_clockwise) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  statist_init(&statist);
  help.turn = 2;
  figure_info.y = 0;
  figure_info.x = 0;
  clockwise_fill(&figure_info, 7);
  cube_to_field(&statist, &figure_info, &help);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);

  ck_assert_int_eq(statist.field[2][1], 1);
  ck_assert_int_eq(statist.field[2][2], 1);
  ck_assert_int_eq(statist.field[2][3], 1);
  ck_assert_int_eq(statist.field[2][4], 1);
  ck_assert_int_eq(figure_info.x, 1);
  statist_close(&statist);
}
END_TEST


START_TEST(test10_line_clockwise) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  statist_init(&statist);
  help.turn = 3;
  figure_info.y = 0;
  figure_info.x = 1;
  clockwise_fill(&figure_info, 6);
  cube_to_field(&statist, &figure_info, &help);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);

  ck_assert_int_eq(statist.field[1][2], 1);
  ck_assert_int_eq(statist.field[2][2], 1);
  ck_assert_int_eq(statist.field[3][2], 1);
  ck_assert_int_eq(statist.field[4][2], 1);
  ck_assert_int_eq(figure_info.y, 1);
  statist_close(&statist);
}
END_TEST


START_TEST(test11_line_clockwise) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  statist_init(&statist);
  help.turn = 4;
  figure_info.y = 1;
  figure_info.x = 1;
  clockwise_fill(&figure_info, 8);
  cube_to_field(&statist, &figure_info, &help);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);

  ck_assert_int_eq(statist.field[2][0], 1);
  ck_assert_int_eq(statist.field[2][1], 1);
  ck_assert_int_eq(statist.field[2][2], 1);
  ck_assert_int_eq(statist.field[2][3], 1);
  ck_assert_int_eq(figure_info.y, 0);
  ck_assert_int_eq(figure_info.x, 0);
  statist_close(&statist);
}
END_TEST


START_TEST(test12_first_fig_init) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  statist_init(&statist);
  figure_info.y = 0;
  figure_info.x = MAP_COLS / 2 - 2;
  first_fig_init(&statist, &figure_info, &help);
  ck_assert_int_eq(help.down_click, 0);
  ck_assert_int_eq(help.floor, 0);
  ck_assert_int_eq(help.turn, 1);
  statist_close(&statist);
}
END_TEST


START_TEST(test13_error_rotate) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  int copy_cubomatrix[QUADR][QUADR] = {0};
  statist_init(&statist);
  help.turn = 2;
  figure_info.y = 0;
  figure_info.x = -2;
  figure_info.cur_figure = I;
  clockwise_fill(&figure_info, 7);
  cube_to_field(&statist, &figure_info, &help);
  copy_cube_to_temp(copy_cubomatrix, &figure_info); 
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);
  error_rotate(&statist, &figure_info, &help);
  clean_carry_cube(&figure_info);
  copy_temp_to_cube(&figure_info, copy_cubomatrix);
  cube_to_field(&statist, &figure_info, &help);
  ck_assert_int_eq(statist.field[0][0], 1);
  ck_assert_int_eq(statist.field[1][0], 1);
  ck_assert_int_eq(statist.field[2][0], 1);
  ck_assert_int_eq(statist.field[3][0], 1);
  ck_assert_int_eq(figure_info.x, -2);
  ck_assert_int_eq(figure_info.y, 0);
  ck_assert_int_eq(help.turn, 2);
  statist_close(&statist);
}
END_TEST


START_TEST(test14_error_rotate) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  int copy_cubomatrix[QUADR][QUADR] = {0};
  statist_init(&statist);
  help.turn = 4;
  figure_info.y = 1;
  figure_info.x = -1;
  figure_info.cur_figure = I;
  clockwise_fill(&figure_info, 8);
  cube_to_field(&statist, &figure_info, &help);
  copy_cube_to_temp(copy_cubomatrix, &figure_info);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);
  error_rotate(&statist, &figure_info, &help);
  clean_carry_cube(&figure_info);
  copy_temp_to_cube(&figure_info, copy_cubomatrix);
  cube_to_field(&statist, &figure_info, &help);
  ck_assert_int_eq(statist.field[1][0], 1);
  ck_assert_int_eq(statist.field[2][0], 1);
  ck_assert_int_eq(statist.field[3][0], 1);
  ck_assert_int_eq(statist.field[4][0], 1);
  ck_assert_int_eq(figure_info.x, -1);
  ck_assert_int_eq(figure_info.y, 1);
  ck_assert_int_eq(help.turn, 4);
  statist_close(&statist);
}
END_TEST



START_TEST(test15_error_rotate) {
  PlayerPos_t figure_info;
  HelpVar_t help;
  GameInfo_t statist;
  int copy_cubomatrix[QUADR][QUADR] = {0};
  statist_init(&statist);
  help.turn = 3;
  figure_info.y = 21;
  figure_info.x = 0;
  figure_info.cur_figure = I;
  clockwise_fill(&figure_info, 6);
  cube_to_field(&statist, &figure_info, &help);
  copy_cube_to_temp(copy_cubomatrix, &figure_info);
  line_clockwise(&statist, &figure_info, &help);
  cube_to_field(&statist, &figure_info, &help);
  error_rotate(&statist, &figure_info, &help);
  clean_carry_cube(&figure_info);
  copy_temp_to_cube(&figure_info, copy_cubomatrix);
  cube_to_field(&statist, &figure_info, &help);
  ck_assert_int_eq(statist.field[23][0], 1);
  ck_assert_int_eq(statist.field[23][1], 1);
  ck_assert_int_eq(statist.field[23][2], 1);
  ck_assert_int_eq(statist.field[23][3], 1);
  ck_assert_int_eq(figure_info.x, 0);
  ck_assert_int_eq(figure_info.y, 21);
  ck_assert_int_eq(help.turn, 3);
  statist_close(&statist);
}
END_TEST


START_TEST(test16_check_full_lines) {
  GameInfo_t statist;
  statist_init(&statist);
  statist.high_score = 0;
  statist.score = 0;
  for (int c = 0; c < MAP_COLS - 1; c++){
    statist.field[22][c] = 2;
  }
  for (int c = 0; c < MAP_COLS; c++){
    statist.field[23][c] = 2;
  }
  check_full_lines(&statist);
  ck_assert_int_eq(statist.score, 100);
  ck_assert_int_eq(statist.high_score, 100);
  for (int c = 0; c < MAP_COLS; c++){
    ck_assert_int_eq(statist.field[22][c], 0);
  }
  for (int c = 0; c < MAP_COLS - 1; c++){
    ck_assert_int_eq(statist.field[23][c], 2);
  }
  statist_close(&statist);
}
END_TEST


START_TEST(test17_check_full_lines) {
  GameInfo_t statist;
  statist_init(&statist);
  statist.high_score = 0;
  statist.score = 0;
  for (int r = 20; r < MAP_ROWS; r++){
    for (int c = 0; c < MAP_COLS; c++){
      statist.field[r][c] = 2;
    }
  }
  check_full_lines(&statist);
  ck_assert_int_eq(statist.score, 1500);
  ck_assert_int_eq(statist.high_score, 1500);
  ck_assert_int_eq(statist.level, 2);
  ck_assert_int_eq(statist.speed, 2);
  for (int r = 20; r < MAP_ROWS; r++){
    for (int c = 0; c < MAP_COLS; c++){
      ck_assert_int_eq(statist.field[r][c], 0);
    }
  }
  statist_close(&statist);
}
END_TEST


START_TEST(test18_check_attach) {
  GameInfo_t statist;
  statist_init(&statist);
  PlayerPos_t figure_info;
  statist.field[23][0] = 1;
  statist.field[23][1] = 1;
  statist.field[23][2] = 1;
  statist.field[23][3] = 1;
  figure_info.x = 0;
  figure_info.y = 21;
  check_attach(&statist, &figure_info);
  
  ck_assert_int_eq(statist.field[23][0], 2);
  ck_assert_int_eq(statist.field[23][1], 2);
  ck_assert_int_eq(statist.field[23][2], 2);
  ck_assert_int_eq(statist.field[23][3], 2);
  statist_close(&statist);
}
END_TEST


START_TEST(test19_check_attach) {
  GameInfo_t statist;
  statist_init(&statist);
  PlayerPos_t figure_info;
  statist.field[4][0] = 1;
  statist.field[4][1] = 1;
  statist.field[4][2] = 1;
  statist.field[4][3] = 1;
  figure_info.x = 0;
  figure_info.y = 2;

  ck_assert_int_eq(check_attach(&statist, &figure_info), 1);

  statist_close(&statist);
}
END_TEST

START_TEST(test20_clear_gameovr_field) {
  GameInfo_t statist;
  statist_init(&statist);
  for (int r = 1; r < MAP_ROWS; r++){
    for (int c = 0; c < MAP_COLS; c++){
      statist.field[r][c] = 1;
    }
  }
  clear_gameovr_field(&statist);
  for (int r = 0; r < MAP_ROWS; r++){
    for (int c = 0; c < MAP_COLS; c++){
      ck_assert_int_eq(statist.field[r][c], 0);
    }
  }
  statist_close(&statist);
}
END_TEST


START_TEST(test21_check_cube_to_field) {
  GameInfo_t statist;
  statist_init(&statist);
  PlayerPos_t figure_info;
  HelpVar_t help;
  help.floor = 0;
  help.down_click = 1;
  figure_info.x = 0;
  figure_info.y = 20;
  remove("high_score.txt");

  for (int r = 0; r < QUADR; r++){
    for (int c = 0; c < QUADR; c++){
      figure_info.cube[r][c] = 0;
    }
  }
  figure_info.cube[2][0] = 1;
  figure_info.cube[2][1] = 1;
  figure_info.cube[2][2] = 1;
  figure_info.cube[2][3] = 1;
  for (int c = 0; c < MAP_COLS - 1; c++){
    statist.field[23][c] = 2;
  }
  figure_info.y++;
  cube_to_field(&statist, &figure_info, &help);
  clear_cubofield(&statist, &figure_info);
  figure_info.y--;
  cube_to_field(&statist, &figure_info, &help);
  help.down_click = 0;
  
  ck_assert_int_eq(statist.field[22][0], 1);
  ck_assert_int_eq(statist.field[22][1], 1);
  ck_assert_int_eq(statist.field[22][2], 1);
  ck_assert_int_eq(statist.field[22][3], 1);
  ck_assert_int_eq(figure_info.x, 0);
  ck_assert_int_eq(figure_info.y, 20);
  ck_assert_int_eq(help.floor, 1);
  statist_close(&statist);
}
END_TEST


Suite *tetris_tests() {
  Suite *result;
  TCase *tc_test;

  result = suite_create("tetris_tests");
  tc_test = tcase_create("tetris_tests");

  tcase_add_test(tc_test, test1_read_high_score);
  tcase_add_test(tc_test, test2_read_high_score);
  tcase_add_test(tc_test, test3_write_high_score);
  tcase_add_test(tc_test, test4_pattern_fill);
  tcase_add_test(tc_test, test5_clear_cubofield);
  tcase_add_test(tc_test, test6_clean_carry_cube);
  tcase_add_test(tc_test, test7_matrix_clockwise);
  tcase_add_test(tc_test, test8_line_clockwise);
  tcase_add_test(tc_test, test9_line_clockwise);
  tcase_add_test(tc_test, test10_line_clockwise);
  tcase_add_test(tc_test, test11_line_clockwise);
  tcase_add_test(tc_test, test12_first_fig_init);
  tcase_add_test(tc_test, test13_error_rotate);
  tcase_add_test(tc_test, test14_error_rotate);
  tcase_add_test(tc_test, test15_error_rotate);
  tcase_add_test(tc_test, test16_check_full_lines);
  tcase_add_test(tc_test, test17_check_full_lines);
  tcase_add_test(tc_test, test18_check_attach);
  tcase_add_test(tc_test, test19_check_attach);
  tcase_add_test(tc_test, test20_clear_gameovr_field);
  tcase_add_test(tc_test, test21_check_cube_to_field);

  suite_add_tcase(result, tc_test);

  return result;
}

int main() {
  int failed = 0;
  Suite *result[] = {tetris_tests(), NULL};

  for (int i = 0; result[i] != NULL; i++) {
    SRunner *runner = srunner_create(result[i]);
    srunner_run_all(runner, CK_NORMAL);
    failed += srunner_ntests_failed(runner);
    srunner_free(runner);
  }
  remove("high_score.txt");
  return (failed == 0) ? EXIT_SUCCESS : CK_FAILURE;
}