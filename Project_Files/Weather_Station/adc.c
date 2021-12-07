/*
 * adc.c
 *
 * Created: 16.11.2021 14:04:26
 *  Author: Mariusz
 */ 



#include <avr/io.h>
#include "adc.h"



void ADC_INIT_AVCC(void)
{
	ADCSRA |= (1<<ADEN); //ADEN: ADC Enable (uruchomienie przetwornika)

	//ADPS2:0: ustawienie preskalera, preskaler= 128 // czestotliwosc pomiarow 125kHz czyli zawiera sie w przedziale podanym w datasheecie
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);

	//Napiecie odiesienia AVCC = 5V
	ADMUX  |=  (1<<REFS0);
}

uint16_t ADC_MEASURE(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | channel;		//wybor kanalu
	ADCSRA |= (1<<ADSC);	//start pojedynczej konwersji
	while(ADCSRA & (1<<ADSC)); //czeka na zakoñczenie konwersji
	return ADC;
}
