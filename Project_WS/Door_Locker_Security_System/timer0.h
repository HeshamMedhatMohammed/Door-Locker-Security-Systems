 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.h
 *
 * Description: Header file for the AVR Timer0 driver
 *
 * Author: Hesham Medhat
 *
 *******************************************************************************/
#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Prescaler;

typedef enum
{
	NORMAL,COMPARE
}Timer0_Mode;

typedef struct
{
	Timer0_Mode Mode;
	uint8 InitialValue;
	uint8 CompValue;
	Timer0_Prescaler prescaler;
}Timer0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer0 driver
 * 	1. Set the required mode.
 * 	2. Set Timer initial value
 * 	3. Set the required prescaler.
 * 	4. Enable Timer0 Overflow Interrupt
 */
void Timer0_init(const Timer0_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to disable the Timer0 to stop the Timer0 Driver
 */
void Timer0_DeInit(void);


#endif /* TIMER0_H_ */
