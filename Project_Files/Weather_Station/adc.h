/*
 * adc.h
 *
 * Created: 16.11.2021 14:05:55
 *  Author: Mariusz
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_INIT_AVCC(void);
uint16_t ADC_MEASURE(uint8_t channel);


#endif /* ADC_H_ */