#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/defines.h"
#include "../../brick_game/tetris/objects.h"

/**
 * @brief Вывод в консоль бортиков поля и дополнительной информации, названий
 * полей статистики, выхода
 */
void print_borders();

/**
 * @brief Печать прямоугольных бортиков
 * @param top_y Верхняя координата прямоугольника по y
 * @param bottom_y Нижняя координата прямоугольника по y
 * @param left_x Левая координата прямоугольника по x
 * @param right_x Правая координата прямоугольника по x
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Печать в консоли "Press enter to start"
 */
void print_start();

/**
 * @brief Отрисовка в консоли фигур на игровом поле в соответствии с массивом
 * field, отрисовка значений статистики
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 */
void draw_field(GameInfo_t *statist);

/**
 * @brief Зачистка в консоли игрового поля пустотой
 */
void clear_field();

/**
 * @brief Печать в консоли сообщения о невозможности вывода на экран
 * счета-рекорда
 */
void print_no_high_score();

/**
 * @brief Зачистка в консоли счета-рекорда, счета и уровня пустотой
 */
void clear_score_level();

/**
 * @brief Отрисовка в консоли следующей сгенерированной фигуры
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 */
void print_next(GameInfo_t statist);

/**
 * @brief Зачистка в консоли поля со следующей сгенерированной фигурой
 */
void clear_next();

/**
 * @brief Печать в консоли счета-рекорда
 * @param statist Структура с текущим состоянием поля, шаблоном следующей
 * фигуры, очков и т.п.
 */
void print_high_score(GameInfo_t statist);

/**
 * @brief Печать в консоли сообщения с окончанием игры
 */
void print_game_over();

/**
 * @brief Печать в консоли сообщения о текущей паузе
 */
void print_pause();

/**
 * @brief Зачистка в консоли поля сообщения о паузе
 */
void clear_pause();

#endif