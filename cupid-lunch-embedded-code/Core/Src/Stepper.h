/*
 * stepper.h
 *
 *  Created on: Sep 28, 2024
 *      Author: Bento
 */
#ifndef STEPPER_H
#define STEPPER_H

#include "main.h"

typedef struct{
	uint16_t step;
	char dir;
	GPIO_TypeDef * GPIOx;
	uint16_t dir_pin;
	TIM_HandleTypeDef *timer;
} Stepper;

void set_speed(double rpm, Stepper *stepper);
void init_stepper(GPIO_TypeDef * GPIOx, uint16_t dir_pin, TIM_HandleTypeDef *timer, Stepper *stepper);

#endif
