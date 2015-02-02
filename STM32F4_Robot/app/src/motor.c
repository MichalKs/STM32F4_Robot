/**
 * @file    motor.c
 * @brief
 * @date    1 lut 2015
 * @author  Michal Ksiezopolski
 *
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */


#include <timers.h>

#include <stm32f4xx.h>

#define MOVE_TIME 100 ///< Movement time in ms

typedef enum {
  STOP = 0,
  FORWARD,
  BACKWARDS,
  LEFT,
  RIGHT,

} MOTOR_MovesTypedef;

MOTOR_MovesTypedef currentMove = STOP;

void MOTOR_Init(void) {

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1
      | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    // output pin
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    // push-pull output
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // less interference
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // no pull-up

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1
      | GPIO_Pin_2 | GPIO_Pin_3);

}

void MOTOR_MoveForward(void) {
  currentMove = FORWARD;
}

void MOTOR_MoveBackwards(void) {
  currentMove = BACKWARDS;
}

void MOTOR_MoveLeft(void) {

  currentMove = LEFT;
}

void MOTOR_MoveRight(void) {

  currentMove = RIGHT;
}

void MOTOR_Stop(void) {
  currentMove = STOP;
}

void MOTOR_Update(void) {

  static uint32_t moveTimer = 0;
  static uint8_t moving = 0;

  // if stop do nothing
  if (currentMove == STOP) {
    GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
    GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3);
    return;
  }

  // not moving
  if (moving == 0) {

    moveTimer = TIMER_GetTime(); // timer for move
    moving = 1; // moving

    // move
    switch (currentMove) {

    case FORWARD:
      GPIO_SetBits(GPIOE, GPIO_Pin_0);
      GPIO_ResetBits(GPIOE, GPIO_Pin_1);

      GPIO_SetBits(GPIOE, GPIO_Pin_2);
      GPIO_ResetBits(GPIOE, GPIO_Pin_3);
      break;
    case BACKWARDS:
      GPIO_SetBits(GPIOE, GPIO_Pin_1);
      GPIO_ResetBits(GPIOE, GPIO_Pin_0);

      GPIO_SetBits(GPIOE, GPIO_Pin_3);
      GPIO_ResetBits(GPIOE, GPIO_Pin_2);
      break;
    case LEFT:
      GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);

      GPIO_SetBits(GPIOE, GPIO_Pin_2);
      GPIO_ResetBits(GPIOE, GPIO_Pin_3);
      break;
    case RIGHT:
      GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3);

      GPIO_SetBits(GPIOE, GPIO_Pin_0);
      GPIO_ResetBits(GPIOE, GPIO_Pin_1);
      break;
    default:
      GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
      GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3);
    }

  } else if (TIMER_DelayTimer(MOVE_TIME, moveTimer)) {
    moving = 0;
    currentMove = STOP;
  }



}

