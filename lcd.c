/********************************************
*
*  Name:  Hyun-Bum Yang
*  Section: Wednesday 2:30
*  Assignment: Lab 7 - Timer
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#define MASKBITS 0xf0  // Top 4 bits
#define RS (1<<PB0) // Register Select
#define ENABLE (1<<PB1) // Enable

/*
  init_lcd - Configure the I/O ports and send the initialization commands
*/
void init_lcd()
{
  DDRD |= 0xf0;               // Set the DDR register bits for ports B and D
  DDRB |= 0x03;

  _delay_ms(15);              // Delay at least 15ms

  writenibble(0x30);          // Use writenibble to send 0011
  _delay_ms(5);               // Delay at least 4msec

  writenibble(0x30);          // Use writenibble to send 0011
  _delay_us(120);             // Delay at least 100usec

  writenibble(0x30);          // Use writenibble to send 0011, no delay needed

  writenibble(0x20);          // Use writenibble to send 0010
  _delay_ms(2);               // Delay at least 2ms
    
  writecommand(0x28);         // Function Set: 4-bit interface, 2 lines

  writecommand(0x0f);         // Display and cursor on

}

/*
  moveto - Move the cursor to the row (0 to 1) and column (0 to 15) specified
*/
void moveto(unsigned char row, unsigned char col)
{
	unsigned char position = 0;
	if (row == 0)
	{
		position = 0x00;
	}
	else if (row == 1)
	{
		position = 0x40;
	}
	position = ((position + col) + 0x80);
	writecommand(position);
}

/*
  stringout - Write the string pointed to by "str" at the current position
*/
void stringout(char *str)
{
	unsigned char i = 0;
	while ((*(str+i)) != '\0')
	{
		writedata(str[i]);
		i++;
	}
}

/*
  writecommand - Send the 8-bit byte "cmd" to the LCD command register
*/
void writecommand(unsigned char cmd)
{
  PORTB &= ~RS; // Set Register Select to Command
  writenibble(cmd);
  writenibble(cmd << 4);
  _delay_ms(2);
}

/*
  writedata - Send the 8-bit byte "dat" to the LCD data register
*/
void writedata(unsigned char dat)
{
  PORTB |= RS;  // Set Register Select to Data
  writenibble(dat);
  writenibble(dat << 4);
  _delay_ms(2);
}

/*
  writenibble - Send four bits of the byte "lcdbits" to the LCD
  Use upper 4 bits
*/
void writenibble(unsigned char lcdbits)
{
  PORTD &= ~MASKBITS; // Clear the top 4 bits
  PORTD |= (lcdbits & MASKBITS);  // Copy the "lcdbits"

  PORTB |= (1<<PB1);  // Enable
  _delay_us(1);
  PORTB &= ~(1<<PB1);
}