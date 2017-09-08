/********************************************
 *
 *  Name:	Hyun-Bum Yang
 *  Section:	Wed. 02:30 - 03:50
 *  Assignment: Final Project
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "encoder.h"
#include "lcd.h"

void init_encoder(void)
{
	change = 0;

    PORTC |= (1 << PC5 | 1 << PC4); // Enable pull-ups on PC4, PC5
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT12) | (1 << PCINT13));

    // Determine the intial state
    bits = PINC;
    a = bits & (1 << PC4);
    b = bits & (1 << PC5);

    if (!b && !a)
	old_state = 0;
    else if (!b && a)
	old_state = 1;
    else if (b && !a)
	old_state = 2;
    else if (a && b)
	old_state = 3;

    new_state = old_state;
}

ISR(PCINT1_vect)
{
	bits = PINC;		// Read the two encoder input at the same time
	a = bits & (1 << PC4);
	b = bits & (1 << PC5);

	// State transitions
	if (old_state == 0) {
	    if (a) {
		new_state = 1;
		if (maxspeed < 99)
		maxspeed++;
	    }
	    else if (b) {
		new_state = 2;
		if (maxspeed > 1)
		maxspeed--;
	    }
	}
	else if (old_state == 1) {
	    if (!a) {
		new_state = 0;
		if (maxspeed > 1)
		maxspeed--;
	    }
	    else if (b) {
		new_state = 3;
		if (maxspeed < 99)
		maxspeed++;
	    }
	}
	else if (old_state == 2) {
	    if (a) {
		new_state = 3;
		if (maxspeed > 1)
		maxspeed--;
	    }
	    else if (!b) {
		new_state = 0;
		if (maxspeed < 99)
		maxspeed++;
	    }
	}
	else if (old_state == 3) {   // old_state = 3
	    if (!a) {
		new_state = 2;
		if (maxspeed < 99)
		maxspeed++;
	    }
	    else if (!b) {
		new_state = 1;
		if (maxspeed > 1)
		maxspeed--;
	    }
	}
	old_state = new_state;	// Update the state
	change = 1;
}