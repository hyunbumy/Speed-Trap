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

#include "serial.h"

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD - 1)

void init_serial(void)
{
	// Enable serial interface data transmission
    UBRR0 = MYUBRR;
    UCSR0B |= (1<<TXEN0 | 1<<RXEN0 | 1<<RXCIE0);
    UCSR0C = (3<<UCSZ00);
}

void data_transfer(int r, char d)
{
	char out[7];
	snprintf(out, 7, "@%d%d$", r, d);
	unsigned char i = 0;
	while ((*(out+i)) != '\0')
	{
		while ((UCSR0A & (1<<UDRE0)) == 0) {}
		UDR0 = (*(out+i));
		i++;
	}
}

ISR(USART_RX_vect)
{
	char x;
	x = UDR0;
	if (x == '@')
	{
		rx_state = 1;
		char_num = 0;
	}
	else if (rx_state == 1)
	{
		if (x <= '9' && x >= '0' && char_num < 4)
		{
			received_string[char_num] = x;
			char_num++;
		}
		else if (x == '$' && char_num > 0)
		{
			received_string[char_num] = '\0';
			received = 1;
			rx_state = 0;
		}
		else	// If no valid input, reset the transmission
		{
			rx_state = 0;
		}
	}
}