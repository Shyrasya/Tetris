#ifndef FSM_H
#define FSM_H

#include <time.h>

#include "../../gui/cli/tetris_frontend.h"
#include "defines.h"
#include "objects.h"
#include "tetris_backend.h"

/**
 * @brief Структура, содержащая всевозможные состояния конечного автомата
 */
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  PAUSE,
  GAMEOVER,
  EXIT_STATE,
} FsmState_t;

/**
 * @brief Структура, содержащая все использующиеся в игре клавиши
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Nosig
} UserAction_t;

/**
 * @brief Функция для обработки нажатия клавиш с клавиатуры и соотнесения её с
 * игровыми клавишами виртуального тетриса
 * @param input Нажатая клавиша клавиатуры
 * @return Виртуальная клавиша тетриса
 */
UserAction_t get_input(int input);

/**
 * @brief Функция для изменения состояния зажатия клавиши, в данном случае
 * кнопки "вниз"
 * @param action Виртуальная клавиша тетриса
 * @param hold Информация об зажатии кнопки
 */
void userInput(UserAction_t action, bool *hold);

/**
 * @brief Функция определяющая дальнейшее развитие игры в зависимости от
 * состояния конечного автомата
 * @param action Нажатая клавиша виртуального тетриса
 * @param fsm_state Текущее состояние конечного автомата
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param figure_info Структура с состоянием игровой фигуры
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_act(UserAction_t *action, FsmState_t *fsm_state, GameInfo_t *statist,
             PlayerPos_t *figure_info, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при "старт" состоянии конечного
 * автомата
 * @param fsm_state Текущее состояние конечного автомата
 */
void fsm_start(FsmState_t *fsm_state);

/**
 * @brief Функция для исполнения действий при состоянии "спавн новой фигуры"
 * конечного автомата
 * @param fsm_state Текущее состояние конечного автомата
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param figure_info Структура с состоянием игровой фигуры
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_spawn(FsmState_t *fsm_state, GameInfo_t *statist,
               PlayerPos_t *figure_info, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при состоянии "движение фигуры"
 * конечного автомата
 * @param action Нажатая клавиша виртуального тетриса
 * @param fsm_state Текущее состояние конечного автомата
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_moving(UserAction_t *action, FsmState_t *fsm_state,
                PlayerPos_t *figure_info, GameInfo_t *statist, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при движении фигуры влево/вправо
 * @param action Нажатая клавиша виртуального тетриса
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_left_right(UserAction_t *action, PlayerPos_t *figure_info,
                    GameInfo_t *statist, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при движении фигуры вниз
 * @param fsm_state Текущее состояние конечного автомата
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_down(FsmState_t *fsm_state, PlayerPos_t *figure_info,
              GameInfo_t *statist, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при состоянии "паузы" конечного
 * автомата
 * @param fsm_state Текущее состояние конечного автомата
 */
void fsm_pause(FsmState_t *fsm_state);

/**
 * @brief Функция для исполнения действий при повороте фигуры
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_rotate(PlayerPos_t *figure_info, GameInfo_t *statist, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при состоянии "сдвиг" конечного
 * автомата
 * @param fsm_state Текущее состояние конечного автомата
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 * @param help Вспомогательная структура для учета положения фигуры при событиях
 */
void fsm_shifting(FsmState_t *fsm_state, PlayerPos_t *figure_info,
                  GameInfo_t *statist, HelpVar_t *help);

/**
 * @brief Функция для исполнения действий при состоянии "прикрепление" конечного
 * автомата
 * @param fsm_state Текущее состояние конечного автомата
 * @param figure_info Структура с состоянием игровой фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 */
void fsm_attaching(FsmState_t *fsm_state, PlayerPos_t *figure_info,
                   GameInfo_t *statist);

/**
 * @brief Функция для исполнения действий при состоянии "игра окончена"
 * конечного автомата
 * @param fsm_state Текущее состояние конечного автомата
 */
void fsm_gameover(FsmState_t *fsm_state);

#endif