/*
 ================================================================================================
 Name        : door_locker_sequrity_system.c
 Author      : Hesham Medhat
 Description : Controlling to unlock a door based on a password
 Date        : 3/11/2021
 ================================================================================================
 */

#define F_CPU 8000000
#include "util/delay.h"
#include <avr/io.h> /* To use the SREG register */
#include "uart.h"
#include "timer0.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "twi_updated.h"

#define PASSWORD_LENGTH 5
#define TRIES_LIMIT 3
#define MATCHED 0
#define UNMATCHED 1
#define MISMATCHES_LIMIT 2
#define OPEN_DOOR 10
#define BUZZER_LOUD 11
#define CHANGE_PASSWORD 12

uint8 g_password0[PASSWORD_LENGTH];
uint8 g_password1[PASSWORD_LENGTH];
volatile uint16 g_incrementer = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * the function which will be called back by the timer0
*/

void APP_counting(void)
{
	g_incrementer++;
}

/*
 * Description :
 * Functional responsible for Generate a new Password by:
 * 1. Receive password twice from UART.
 * 2. Compare them.
 * 3. if matched save them in EEPROM if not repeat itself.
 */

void APP_generatePassword(void)
{
	uint8 a_counter;
	uint8 a_check = 0;
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password0[a_counter] = UART_recieveByte();
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password1[a_counter] = UART_recieveByte();
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		if (g_password0[a_counter] == g_password1[a_counter])
		{
			a_check++;
		}
	}
	if (a_check == PASSWORD_LENGTH)
	{
		UART_sendByte(MATCHED);
		for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
		{
			EEPROM_writeByte(0x0311+a_counter, g_password0[a_counter]); /* Save Password in the external EEPROM */
			_delay_ms(10);
		}
	}
	else
	{
		UART_sendByte(UNMATCHED);
		APP_generatePassword();
	}
}

/*
 * Description :
 * Functional responsible for Check if the Password is Correct or not by:
 * 1. receive password from UART.
 * 2. Compare it with the Password in EEPROM.
 * 3. Get the results and for matched return with 0 for unmatched repeat itself three times before return 2.
 */

uint8 APP_checkPassword(void)
{
	uint8 a_counter;
	uint8 a_check = 0;
	static uint8 a_triesCheck = 0;
	uint8 a_returnCase;
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password0[a_counter] = UART_recieveByte();
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		EEPROM_readByte(0x0311+a_counter, &(g_password1[a_counter]));  /* Read Password from the external EEPROM */;
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		if (g_password0[a_counter] == g_password1[a_counter])
		{
			a_check++;
		}
	}
	if (a_check == PASSWORD_LENGTH)
	{
		UART_sendByte(MATCHED);
		return MATCHED;
	}
	else
	{
		a_triesCheck++;
		if (a_triesCheck < TRIES_LIMIT)
		{
			UART_sendByte(UNMATCHED);
			a_returnCase = APP_checkPassword();
			a_triesCheck=0;
			return a_returnCase;
		}
		else
		{
			UART_sendByte(MISMATCHES_LIMIT);
			a_triesCheck=0;
			return MISMATCHES_LIMIT;
		}
	}
}

/*
 * Description :
 * Functional responsible for Open the door by the motor.
 */

void APP_openDoor(void)
{
	Timer0_ConfigType Timer0_Config = {NORMAL,0x00,0xFF,F_CPU_1024};

	Timer0_init(&Timer0_Config);
	DcMotor_Rotate(DC_MOTOR_CW);
	while (g_incrementer < 458){}
	g_incrementer = 0;
	Timer0_DeInit();

	Timer0_init(&Timer0_Config);
	DcMotor_Rotate(DC_MOTOR_STOP);
	while (g_incrementer < 92){}
	g_incrementer = 0;
	Timer0_DeInit();

	Timer0_init(&Timer0_Config);
	DcMotor_Rotate(DC_MOTOR_ACW);
	while (g_incrementer < 458){}
	g_incrementer = 0;
	Timer0_DeInit();

	DcMotor_Rotate(DC_MOTOR_STOP);
}

/*
 * Description :
 * Functional responsible for Generate alarm on the buzzer when mismatch the password 3 times.
 */

void APP_buzzerON(void)
{
	Timer0_ConfigType Timer0_Config = {NORMAL,0x00,0xFF,F_CPU_1024};

	Timer0_init(&Timer0_Config);
	BUZZER_ON();
	while (g_incrementer < 1832){}
	g_incrementer = 0;
	Timer0_DeInit();

	BUZZER_OFF();
}

/*
 * Description :
 * Functional responsible for Main menu options by:
 * 1. Check the password.
 * 2. Take option.
 * 3. if password is matched do the order if not show the error.
 */

void APP_options(void)
{
	uint8 a_passwordCase;
	uint8 a_option;
	a_passwordCase = APP_checkPassword();
	a_option = UART_recieveByte();
	if (a_option == OPEN_DOOR)
	{
		APP_openDoor();
	}
	else if (a_option == BUZZER_LOUD)
	{
		APP_buzzerON();
	}
	else if (a_option == CHANGE_PASSWORD)
	{
		APP_generatePassword();
	}
}

int main()
{
	Uart_ConfigType Uart_Config = {_8_BIT,DISABLE,_1_BIT,9600};
	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	Twi_ConfigType Twi_Config = {_400_kbs,0x01};
	UART_init(&Uart_Config);
	TWI_init(&Twi_Config);
	DcMotor_Init();
	BUZZER_init();
	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);
	Timer0_setCallBack(APP_counting);

	APP_generatePassword();
	while(1)
	{
		APP_options();
	}
}
