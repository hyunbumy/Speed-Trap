/********************************************
 *
 *  Name:	Hyun-Bum Yang
 *  Section:	Wed. 02:30 - 03:50
 *  Assignment: Final Project
 *
 ********************************************/

volatile unsigned char bits, a, b;
volatile unsigned char new_state, old_state;
volatile unsigned char change;	// Flag for rotary change
volatile unsigned char maxspeed;		// Count to display

void init_encoder(void);
ISR(PCINT1_vect);
