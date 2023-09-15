/*
 * main.c
 *
 * Created: 10/31/2022 7:13:58 PM
 *  Author: DarkStar
 */ 

#include <xc.h>
#include "prog.h"
#include <avr/interrupt.h>

int main(void)
{
	cli();
	init();
	WelcomeScreen();
	while(1)
	{
		Progmanager();
	}
}