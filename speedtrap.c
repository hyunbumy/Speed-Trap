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
#include <avr/eeprom.h>

#include "lcd.h"
#include "encoder.h"
#include "timer.h"
#include "speedtrap.h"
#include "serial.h"

int main(void)
{
	sei();
	init_main();
	init_lcd();
	init_timer1(16000);	// Initialize timer that ticks every 1 millisecond
	init_encoder();
	init_serial();
	writecommand(1);

	unsigned char temp = eeprom_read_byte((void *) 100);	// Read from EEPROM address 100
	if (temp >= 1 && temp <= 99)
	{
		maxspeed = temp;
	}

	char tout[10];
	snprintf(tout, 10, "Limit: %2u", maxspeed);
	moveto(1,0);
	stringout(tout);

	while(1)
	{
		if (change)	// When there is a change in rotary encoder
		{
			eeprom_update_byte((void *) 100, maxspeed);	// Write maxspeed on EEPROM when changed
			char speed[3];
			snprintf(speed, 3, "%2u", maxspeed);
			moveto(1,7);
			stringout(speed);
			change = 0;
		}

		if (received)	// When correct speed is received
		{
			int rx_speed;
			sscanf(received_string, "%d", &rx_speed);
			moveto(0,6);
			char t2[6];
			snprintf(t2, 6, "%3d.%d", rx_speed/10, rx_speed%10);
			stringout(t2);	// Display received speed
			if (rx_speed > (maxspeed*10))	// Compare the received speed with maxspeed
			{
				buzz();
			}
			char_num = 0;	// Reset the number of characters
			received = 0;	// Clear RX flag
		}

		if (is_done)	// When speed measurement is finished
		{
			moveto(0,0);

			// Conversion factor to cm/s with LEDs 1.4 in apart and t as milisecond
			// Then multiply the factor by 10 to take the first decimal point value into consideration
			unsigned int vel = 35560/t;
			if (vel > 9999)
			{
				vel = 9999;
			}
			char decimal = vel%10;
			int real = vel/10;
			char tout[6];
			snprintf(tout, 6, "%3d.%d", real, decimal);
			stringout(tout);	// Display local speed
			snprintf(tout, 5, "%4d", t);
			moveto(1,12);
			stringout(tout);	// Display local time
			data_transfer(real, decimal);
			is_done = 0;	// clear flag
		}
	}
}

void buzz(void)
{
	int i;
	for (i = 0; i < 500; i++)
	{
		PORTC |= (1 << PC2);
    	_delay_us(500);
    	PORTC &= ~(1 << PC2);
    	_delay_us(500);
	}
}

void init_main(void)
{
	state = 0;
	t = 0;
	maxspeed = 30;
	is_done = 0;
	received = 0;
	rx_state = 0;
	char_num = 0;

	DDRC |= ((1<<PC1)|(1<<PC2));	// State indicator LED and buzzer port as output
	PORTC &= ~(1<<PC1);	// Initially off
	PORTB |= ((1<<PB3) | (1<<PB4));	// Pull-up resistors for phototransistors
	DDRD |= (1<<PD2);	// Tri-state gate to output
	PORTD &= ~(1<<PD2);	// Enable data transmission

    PCICR |= (1 << PCIE0);	// Initialize PORTB interrupts
    PCMSK0 |= ((1 << PCINT3) | (1 << PCINT4));	// Enable bit 3, 4 interrupt
}

ISR(PCINT0_vect)
{
	if ((PINB & (1<<PB4)) == 0 && state == 0)
	{
		PORTC |= (1<<PC1);	// Turn on state LED
		t = 0;	// Reset time
		state = 1;
	}
	else if ((PINB & (1<<PB3)) == 0 && state == 1)
	{
		PORTC &= ~(1<<PC1);	// Turn off state LED
		state = 0;
		is_done = 1;
	}
}