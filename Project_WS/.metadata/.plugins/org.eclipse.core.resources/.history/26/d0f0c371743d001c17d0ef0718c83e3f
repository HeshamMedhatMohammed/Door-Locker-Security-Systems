 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER
 *
 * Author: Hesham Medhat
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad Port Configurations */
#define BUZZER_PORT_ID                  PORTA_ID
#define BUZZER_PIN_ID                   PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the BUZZER
 * 1. Setup the direction of BUZZER pin as output using GPIO driver.
 * 2. Initialize it with zero (BUZZER is OFF)
 */
void BUZZER_init();

/*
 * Description :
 * Set BUZZER pin = 1 using GPIO driver.
 */
void BUZZER_ON();

/*
 * Description :
 * Clear BUZZER pin = 0 using GPIO driver.
 */
void BUZZER_OFF();

#endif /* BUZZER_H_ */
