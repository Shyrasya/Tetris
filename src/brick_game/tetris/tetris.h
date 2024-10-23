#ifndef TETRIS_H
#define TETRIS_H

#include <locale.h>
#include <time.h>

#include "../../gui/cli/tetris_frontend.h"
#include "fsm.h"
#include "tetris_backend.h"

/**
 * @brief Основной игровой цикл игры
 */
void game_cycle();

/**
 * @brief Функция с обработкой текущего состояния конечного автомата и игрового
 * процесса, и последующей отрисовкой интерфейса
 * @param fsm_state Текущее состояние конечного автомата
 */
GameInfo_t updateCurrentState(FsmState_t *fsm_state);

/**
 * @brief Вспомогательна функция updateCurrentState для обработки "START"
 * состояния конечного автомата
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param fsm_state Текущее состояние конечного автомата
 */
void start_upd_cur_st(GameInfo_t *statist, FsmState_t *fsm_state);

/**
 * @brief Вспомогательна функция updateCurrentState для обработки "GAME_OVER"
 * состояния конечного автомата
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 */
void gameover_upd_cur_st(GameInfo_t *statist);

#endif