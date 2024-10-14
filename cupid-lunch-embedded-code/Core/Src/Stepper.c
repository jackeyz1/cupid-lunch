/*
 * stepper.c
 *
 *  Created on: Sep 28, 2024
 *      Author: Bento
 */
#include "Stepper.h"
#include "math.h"

#define MAX_MOTOR 100

Stepper* Motors[MAX_MOTOR];

uint8_t motor_cnt = 0;

void set_speed(double rpm, Stepper *stepper){
	if(rpm>0){
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->dir_pin,GPIO_PIN_SET);
		stepper->dir = 1;
	}
	else{
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->dir_pin, GPIO_PIN_RESET);
		stepper->dir = 0;
	}

	double sps = fabs(rpm)*200.0*16.0/60.0;
//	if(fabs(rpm)<0.01){
//		stepper->timer->Instance->CCR1 = 0;
//		stepper->timer->Instance->ARR  = 0;
//		return;
//	}



	if(fabs(sps)<32){
		//stepper->timer->Instance->CNT = 0;
		stepper->timer->Instance->ARR = 0;
		stepper->timer->Instance->CCR1 = 0;
		HAL_TIM_Base_Stop_IT(stepper->timer);
		HAL_TIM_PWM_Stop(stepper->timer, TIM_CHANNEL_1);
		return;
	}
	//stepper->timer->Instance->CNT = 0;
	stepper->timer->Instance->ARR = (uint16_t)(2000000.0/fabs(sps));
	stepper->timer->Instance->CCR1 = stepper->timer->Instance->ARR*0.1;\
	if(!stepper->timer->Instance->CR1&TIM_CR1_CEN){
		HAL_TIM_Base_Start_IT(stepper->timer);
		HAL_TIM_PWM_Start(stepper->timer, TIM_CHANNEL_1);
	}


}

void init_stepper(GPIO_TypeDef * GPIOx, uint16_t dir_pin, TIM_HandleTypeDef *timer, Stepper *stepper){
	stepper->GPIOx = GPIOx;
	stepper->dir_pin = dir_pin;
	stepper->timer = timer;
	stepper->step = 0;
	stepper->dir = 0;
	HAL_TIM_Base_Start_IT(stepper->timer);
	HAL_TIM_PWM_Start(stepper->timer, TIM_CHANNEL_1);
	Motors[motor_cnt] = stepper;
	motor_cnt++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{

	for(int i=0;i<motor_cnt;i++){
		if(htim->Instance == Motors[i]->timer->Instance){
			if(Motors[i]->dir){
				Motors[i]->step++;
				if(Motors[i]->step == 16*200){
					Motors[i]->step = 0;
				}
			}
			else{
				if(Motors[i]->step == 0){
					Motors[i]->step = 16*200;
				}
				Motors[i]->step--;

			}
		}
	}


}
