/*
 * TrybCTC.c
 * Timer generuj¹cy przerwanie co 1 sekunde
 * Created: 10.11.2021 15:02:21
 * Author : Mariusz
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>	

#include "TimecrCTC.h"

void TimerCTC_INIT(void)
{
    TCCR1B |= (1<<CS12)|(1<<CS10); //clk/1024 (From prescaler) => 16 000 000 / 1024 = 15 625 Hz
	TCCR1B |= (1<<WGM12); // TRYB CTC	
	OCR1A = 15625; //Przerwanie od porównania zawartoœci licznika sprzetowego Timera z rejestrem OCR1A

	//Wlaczamy przerwanie od TIMER1 i ogolnie przerwania
	TIMSK1 |= (1<<OCIE0A);	//Enable TIMER1_COMPA
	sei (); // Enable interrupts
}


