#pragma once

//Размер по умолчанию
#define TB_HINT_WIDTH 500

//Временные константы
#define SECONDS_IN_MINUTE   60
#define SECONDS_IN_HOUR     3600
#define SECONDS_IN_DAY      86400

//Размеры частей виджета
#define TB_AVAILABILITY_AREA_HEIGHT 6
#define TB_TIME_SCALE_AREA_HEIGHT   40
#define TB_TIME_SLIDER_HEIGHT       12
#define TB_TIME_BAR_HEIGHT          65

//Размеры делений
#define TB_DIVISION_MIN_WIDTH       6
#define TB_DIVISION_HEIGHT_YEAR     37
#define TB_DIVISION_HEIGHT_BIG      25
#define TB_DIVISION_HEIGHT_MEDIUM   13
#define TB_DIVISION_HEIGHT_MINOR    8

//Шаги масштабирования по изменению цены деления
#define TB_SCALE_MAX    24 //18
#define TB_SCALE_S1     0
#define TB_SCALE_S10    4
#define TB_SCALE_M1     8
#define TB_SCALE_M10    12
#define TB_SCALE_H1     16
#define TB_SCALE_H6     20
#define TB_SCALE_D1     24

//Минимальный период подписей на шкале
#define TB_SCALE_S1_MIN_DIV_SIGN    20
#define TB_SCALE_S10_MIN_DIV_SIGN   120
#define TB_SCALE_M1_MIN_DIV_SIGN    1200
#define TB_SCALE_M10_MIN_DIV_SIGN   7200
#define TB_SCALE_H1_MIN_DIV_SIGN    43200
#define TB_SCALE_H6_MIN_DIV_SIGN    345600
#define TB_SCALE_D1_MIN_DIV_SIGN    1382400

//Цены деления
#define TB_DIV_VALUE_S1     1
#define TB_DIV_VALUE_S10    10
#define TB_DIV_VALUE_M1     60
#define TB_DIV_VALUE_M10    600
#define TB_DIV_VALUE_H1     3600
#define TB_DIV_VALUE_H6     21600
#define TB_DIV_VALUE_D1     86400
