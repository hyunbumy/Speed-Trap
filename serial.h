/********************************************
 *
 *  Name:	Hyun-Bum Yang
 *  Section:	Wed. 02:30 - 03:50
 *  Assignment: Final Project
 *
 ********************************************/

volatile unsigned char rx_state;
volatile unsigned char char_num;
volatile unsigned char received;
volatile char received_string[5];

void data_transfer(int r, char d);
void init_serial(void);