/*
 ================================================================================================
 Name        : door_locker_sequrity_system.c
 Author      : Hesham Medhat
 Description : Interface with the user
 Date        : 3/11/2021
 ================================================================================================
 */

#define F_CPU 8000000
#include "util/delay.h"
#include <avr/io.h> /* To use the SREG register */
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "timer0.h"

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
 * 1. Take password twice from Keypad.
 * 2. Send them by the UART.
 * 3. Get the results and for matched send by UART to save it for unmatched repeat itself.
 */
void APP_generatePassword(void)
{
	uint8 a_counter;
	uint8 a_check;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"PLZ Enter Pass");
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password0[a_counter] = KEYPAD_getPressedKey();
		LCD_displayStringRowColumn(1,a_counter,"*");
		_delay_ms(400); /* delay to take only one charachter in a single press on Keypad */
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"PLZ ReEnter Pass");
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password1[a_counter] = KEYPAD_getPressedKey();
		LCD_displayStringRowColumn(1,a_counter,"*");
		_delay_ms(400); /* delay to take only one charachter in a single press on Keypad */
	}

	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		UART_sendByte(g_password0[a_counter]);
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		UART_sendByte(g_password1[a_counter]);
	}

	a_check = UART_recieveByte();
	if (a_check == MATCHED)
	{
		return;
	}
	else if (a_check == UNMATCHED)
	{
		APP_generatePassword();
	}
}

/*
 * Description :
 * Functional responsible for Check if the Password is Correct or not by:
 * 1. Take password from Keypad.
 * 2. Send them by the UART.
 * 3. Get the results and for matched return with 0 for unmatched repeat itself three times before return 2.
 */
uint8 APP_checkPassword(void)
{
	uint8 a_counter;
	uint8 a_check ;
	uint8 a_returnCase;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Enter Pass");
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		g_password0[a_counter] = KEYPAD_getPressedKey();
		LCD_displayStringRowColumn(1,a_counter,"*");
		_delay_ms(400); /* delay to take only one charachter in a single press on Keypad */
	}
	for (a_counter=0; a_counter < PASSWORD_LENGTH; a_counter++)
	{
		UART_sendByte(g_password0[a_counter]);
	}
	a_check = UART_recieveByte();
	if (a_check == MATCHED)
	{
		return MATCHED;
	}
	else if (a_check == UNMATCHED)
	{
		a_returnCase = APP_checkPassword();
		return a_returnCase;
	}
	else if (a_check == MISMATCHES_LIMIT)
	{
		return MISMATCHES_LIMIT;
	}
}

/*
 * Description :
 * Functional responsible for Show the door state on LCD.
 */
void APP_openDoor(void)
{
	Timer0_ConfigType Timer0_Config = {NORMAL,0x00,0xFF,F_CPU_1024};

	Timer0_init(&Timer0_Config);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Unlocking door");
	while (g_incrementer < 458){} /* wait for 15 Second */
	g_incrementer = 0;
	Timer0_DeInit();

	Timer0_init(&Timer0_Config);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door is unlocked");
	while (g_incrementer < 92){} /* wait for 3 Second */
	g_incrementer = 0;
	Timer0_DeInit();

	Timer0_init(&Timer0_Config);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"locking door");
	while (g_incrementer < 458){} /* wait for 15 Second */
	g_incrementer = 0;
	Timer0_DeInit();
}

/*
 * Description :
 * Functional responsible for Generate error message for a minute when mismatch the password 3 times.
 */
void APP_buzzerON(void)
{
	Timer0_ConfigType Timer0_Config = {NORMAL,0x00,0xFF,F_CPU_1024};

	Timer0_init(&Timer0_Config);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"ERROR MESSAGE");
	while (g_incrementer < 1832){} /* wait for one minute */
	g_incrementer = 0;
	Timer0_DeInit();
}

/*
 * Description :
 * Functional responsible for Main menu options by:
 * 1. Show the available options and take the choice
 * 2. Take password from Keypad and check it.
 * 3. if matched do the order if not show the error.
 */
void APP_options(void)
{
	uint8 a_choice;
	uint8 a_passwordCase;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+:Open door");
	LCD_displayStringRowColumn(1,0,"-:Change pass");
	a_choice = KEYPAD_getPressedKey();
	_delay_ms(400); /* delay to take only one charachter in a single press on Keypad */
	if (a_choice == '+')
	{
		a_passwordCase = APP_checkPassword();
		if (a_passwordCase == MATCHED)
		{
			UART_sendByte(OPEN_DOOR);
			APP_openDoor();
		}
		else if (a_passwordCase == MISMATCHES_LIMIT)
		{
			UART_sendByte(BUZZER_LOUD);
			APP_buzzerON();
		}
	}
	else if (a_choice == '-')
	{
		a_passwordCase = APP_checkPassword();
		if (a_passwordCase == MATCHED)
		{
			UART_sendByte(CHANGE_PASSWORD);
			APP_generatePassword();
		}
		else if (a_passwordCase == MISMATCHES_LIMIT)
		{
			UART_sendByte(BUZZER_LOUD);
			APP_buzzerON();
		}
	}
	else
	{
		APP_options();
	}
}

int main(void)
{
	/* Initialize all the hardware required in these microcontroller */
	LCD_init();
	Uart_ConfigType Uart_Config = {_8_BIT,DISABLE,_1_BIT,9600};
	UART_init(&Uart_Config);

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);
	Timer0_setCallBack(APP_counting);
	/* Generate the Password at the start of the Program */
	APP_generatePassword();

	while(1)
	{
		/* Stay in the options until turn off the system */
		APP_options();
	}
}
