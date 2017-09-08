/********************************************
 *
 *  Name:	Hyun-Bum Yang
 *  Section:	Wed. 02:30 - 03:50
 *  Assignment: Final Project
 *
 ********************************************/

volatile unsigned int t;
volatile unsigned char state;
void init_timer1(unsigned short);
ISR(TIMER1_COMPA_vect);