 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER
 *
 * Author: Hesham Medhat
 *
 *******************************************************************************/

#include "buzzer.h"


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the BUZZER
 * 1. Setup the direction of BUZZER pin as output using GPIO driver.
 * 2. Initialize it with zero (BUZZER is OFF)
 */
void BUZZER_init()
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, 0);
}

/*
 * Description :
 * Set BUZZER pin = 1 using GPIO driver.
 */
void BUZZER_ON()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, 1);
}

/*
 * Description :
 * Clear BUZZER pin = 0 using GPIO driver.
 */
void BUZZER_OFF()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, 0);
}
