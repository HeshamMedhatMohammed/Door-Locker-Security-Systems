 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.c
 *
 * Description: Source file for the AVR Timer0 driver
 *
 * Author: Hesham Medhat
 *
 *******************************************************************************/

#include "timer0.h"
#include <avr/io.h> /* To use Timer0 Registers */
#include <avr/interrupt.h> /* For timer0ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the time is over */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the time is over */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

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
void Timer0_init(const Timer0_ConfigType * Config_Ptr)
{
	TCNT0 = (Config_Ptr->InitialValue); /* Set Initial Value */
	if ((Config_Ptr->Mode) == NORMAL)
	{
		TIMSK |= (1<<TOIE0); /* Enable Timer0 Overflow Interrupt */
		/* Configure the timer control register
		 * 1. Non PWM mode FOC0=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. Normal Mode COM00=0 & COM01=0
		 */
		TCCR0 = (1<<FOC0);
	}
	else if ((Config_Ptr->Mode) == COMPARE)
	{
		OCR0  = (Config_Ptr->CompValue); /* Set Compare Value */
		TIMSK |= (1<<OCIE0); /* Enable Timer0 Compare Interrupt */
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. CTC Mode WGM01=1 & WGM00=0
		 * 3. No need for OC0 so COM00=0 & COM01=0
		 */
		TCCR0 = (1<<FOC0) | (1<<WGM01);
	}
	TCCR0 = (TCCR0 & 0xF8) | ((Config_Ptr->prescaler) & 0x07);
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
		g_callBackPtr = a_ptr;
}

/*
 * Description: Function to disable the Timer0 to stop the Timer0 Driver
 */
void Timer0_DeInit(void)
{
	/* Clear All Timer0 Registers */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0  = 0;
		TIMSK &= ~(1<<TOIE0);
		TIMSK &= ~(1<<OCIE0);
}

