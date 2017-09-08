/********************************************
*
*  Name:	Hyun-Bum Yang
*  Lab section:	Wed. 2:30 - 3:50
*  Assignment:  Lab 4 - ADC
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

void variable_delay_us(int);

unsigned short period[13] =     // Periods in microseconds for 13 tones
{ 7634, 6803, 6061, 5405, 4808, 4292, 3817, 3401, 3030, 2703, 2410, 2146, 1912 };

int main(void)
{
    // Variable Declaration
    unsigned int half_period = 0;
    int i, j;
    int range;

    // Initialize appropriate DDR and PORT registers
    DDRB |= (1 << DDB4);

    // Initialize the ADC
    ADMUX |= (1 << REFS0);  // Set Voltage Reference to AVCC
    ADMUX |= ((1 << MUX0) | (1 << MUX1));   // AN3 or PC3
    ADMUX |= (1 << ADLAR);  // Data storage location (8-bit)

    ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); // Set the prescale
    ADCSRA |= (1 << ADEN);  //Enable ADC

    while (1)       // Loop forever
    {   
        // Start an ADC conversion
        ADCSRA |= (1 << ADSC);	// Start the conversion
        while ((ADCSRA & (1<<ADSC)) != 0) {}	// Check the completion
        unsigned char result = ADCH;

    	//	Check the range of periods
	    range = result / 20;
	    half_period = period[range] / 2;

        // Generate 10 periods of the output tone
        for (j = 0; j < 1; j++)
        {
            PORTB |= (1 << PB4);
            variable_delay_us(half_period);
            PORTB &= ~(1 << PB4);
            variable_delay_us(half_period);
        }
    }

    return 0;   /* never reached */
}

/*
  variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;
    
    while (i--)
	_delay_us(10);
}