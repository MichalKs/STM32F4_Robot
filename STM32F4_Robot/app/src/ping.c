/**
 * @file    ping.c
 * @brief   HC-SR04 ultrasound distance sensor library
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
#include <stdio.h>

#ifndef DEBUG
  #define DEBUG
#endif

#ifdef DEBUG
  #define print(str, args...) printf("PING--> "str"%s",##args,"\r")
  #define println(str, args...) printf("PING--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

#define ECHO_PIN GPIO_Pin_4
#define TRIG_PIN GPIO_Pin_5
#define US_TO_CM (340.0*100/2/1000000)

/**
 * @brief Initialize ping sensor
 */
void PING_Init(void) {

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin   = TRIG_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    // output pin
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;    // push-pull output
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // less interference
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // no pull-up

  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin   = ECHO_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;    // output pin
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // no pull-up

  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void PING_Read(void) {

  GPIO_SetBits(GPIOB, TRIG_PIN);
  TIMER_DelayUS(10);
  GPIO_ResetBits(GPIOB, TRIG_PIN);

  uint32_t us = 0;
  while(!GPIO_ReadInputDataBit(GPIOB, ECHO_PIN));

  while(GPIO_ReadInputDataBit(GPIOB, ECHO_PIN)) {
    TIMER_DelayUS(10);
    us+=10;
  }

  double distance = (double)us * 0.01715;

  println("Microseconds = %u, distance in cm = %02f", us, distance);



}
