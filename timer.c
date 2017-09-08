/********************************************
 *
 *  Name:	Hyun-Bum Yang
 *  Section:	Wed. 02:30 - 03:50
 *  Assignment: Final Project
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "timer.h"

//Timer initialization
void init_timer1(unsigned short m)
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = m;	// Milisecond which is equivalent to 16000
	TCCR1B |= (1 << CS10);	// Prescalar 1
}

/*
ISR is called only when the state goes from 0 (stopped) to 1 (running)
In 0, displays initial screen, resets the time, and disables timer (Stop)
In 1, the updated time is displayed every 0.1s (Running)
In 2, the time is kept in record, but does not display updated value (Lap)
*/
ISR(TIMER1_COMPA_vect)
{
	if (state == 1)	// Running State
	{
		t++;
	}
}