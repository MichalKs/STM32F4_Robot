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


#include <stm32f4xx.h>


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
  GPIO_SetBits(GPIOE, GPIO_Pin_0);
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}

void MOTOR_MoveBackwards(void) {
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
  GPIO_ResetBits(GPIOE, GPIO_Pin_0);
}

void MOTOR_Stop(void) {
  GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
}


