#include <avr/io.h>
#include "adc.h"

void ADC_INIT_AVCC(void)
{
	ADCSRA |= (1<<ADEN); //ADEN: ADC Enable (start converter)

	//ADPS2:0: prescaler setting, prescaler = 128 // measurement frequency 125kHz, i.e. within the range given in the datasheet
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);

	//Reference voltage AVCC = 5V
	ADMUX  |=  (1<<REFS0);
}

uint16_t ADC_MEASURE(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | channel;		// channel choice
	ADCSRA |= (1<<ADSC);	// start simple conversion
	while(ADCSRA & (1<<ADSC)); // wait to stop conversion
	return ADC;
}
