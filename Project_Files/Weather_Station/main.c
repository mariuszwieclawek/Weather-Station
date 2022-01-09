/*
 * Weather_Station.c
 *
 * Created: 15.11.2021 15:50:08
 * 
 */ 

#define F_CPU 16000000UL // Clock Speed

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "terminal.h"
#include "adc.h"
#include "DHT11.h"
#include "EEPROM.h"
#include "TimecrCTC.h"

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

//#define ADCIN PORTC2   //definicja ADCIN (wej?cie ADC2), PRZETWORNIK ADC2 NA PORCIE PORTC2

uint8_t c=0,I_HUM,D_HUM,I_Temp,D_Temp,CheckSum;

unsigned int EEPROM_ADDRESS = 0x0000;		// address where we save the data measurements in eeprom memory

uint16_t one_sec = 0; //seconds counter
uint16_t ten_min = 0;


#define LED (1<<PORTB5)		// Built-in led on PORTB5
#define LED_ON PORTB |= LED
#define LED_OFF PORTB &= ~LED
#define LED_TOG PORTB ^= LED


ISR(TIMER1_COMPA_vect) // Interrupt from comparing the contents of the Timer's hardware counter with the OCR1A register
{ 
	one_sec++;
	if (one_sec == 5)
	{
		ten_min = 1;
		one_sec = 0;
	}
}


int main(void)
{
	cli();
	USART_Init(MYUBRR);		// initialization USART
	ADC_INIT_AVCC();		// initialization converter ADC
	TimerCTC_INIT();		// initialization timer
	
	/* Start */
	tr_cls(0);
	USART_PutS("Pomiar wilgotnosci, temperatury oraz natezenia swiatla: ");
	_delay_ms(2000);
	tr_cls(1);
	
	//DDRC &=	~(1<<ADCIN);            //Ustawienie PORTC2 jako wejscie (przetwornik ADC)
	double meas1 = 0;				// ADC on channel PC1
	double meas2 = 0;				// ADC on channel PC2
	
	DDRB |= LED;
	
	sei();
	while(1)
	{
		/************************************************			ADC				*************************************************************/
		
		/********************* ADC1 RAINDROPS MEASUREMENT************************/ 
		meas1 = ADC_MEASURE(PORTC1);
		
		// ADC1 RAINDROPS AS A VOLTAGE
		meas1 = (5 * meas1) / 1024;			// we scale as voltage, 1024 because ADC 10bit
		meas1 *= 100;							// we shift it by two places to change the format from x, xx to xxx
		div_t divmod1 = div(meas1,100);		// we create a structure in which we save the result and the remainder from division by 100 divmod.quot-result, divmod.rem-remainder
		
		//WRITE MEASUREMENTS TO STRUCTURE		//DATA STRUCTURE DEFINE IN EEPROM.h
		Data_Measurements.I_sensor1 = divmod1.quot;			// integers
		Data_Measurements.D_sensor1 = divmod1.rem;			// fractional
		
		/********************* ADC2 LIGHT INTENSITY MEASUREMENT************************/
		// ADC2 LIGHT INTENSITY MEASUREMENT
		meas2 = ADC_MEASURE(PORTC2);
		
		// ADC2 LIGHT INTENSITY AS A VOLTAGE
		meas2 = (5 * meas2) / 1024;			// we scale as voltage, 1024 because ADC 10bit
		meas2 *= 100;							// we shift it by two places to change the format from x, xx to xxx
		div_t divmod2 = div(meas2,100);		// we create a structure in which we save the result and the remainder from division by 100 divmod.quot-result, divmod.rem-remainder
		
		//WRITE MEASUREMENTS TO STRUCTURE		//DATA STRUCTURE DEFINE IN EEPROM.h
		Data_Measurements.I_sensor2 = divmod2.quot;			// integers
		Data_Measurements.D_sensor2 = divmod2.rem;			// fractional
		
		
		/************************************************			DHT11				*************************************************************/
		_delay_ms(300);
		DHT_Request();		/* send start pulse */
		DHT_Response();		/* receive response */
		I_HUM=DHT_Receive_data();	/* store first eight bit in I_HUM */
		D_HUM=DHT_Receive_data();	/* store next eight bit in D_HUM */
		I_Temp=DHT_Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=DHT_Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=DHT_Receive_data();/* store next eight bit in CheckSum */
		
		//WRITE MEASUREMENTS TO STRUCTURE		//DATA STRUCTURE DEFINE IN EEPROM.h
		Data_Measurements.D_Temperature = D_Temp;
		Data_Measurements.I_Temperature = I_Temp;
		Data_Measurements.I_Humidity = I_HUM;
		Data_Measurements.D_Humidity = D_HUM;
		
		
		/************************************************		   EEPROM READ AND WRITE			*************************************************************/
		cli();
		if(ten_min == 1) // if 10 minutes have passed
		{
			LED_TOG; // change of state to the opposite diode
			write_all_to_EEPROM(EEPROM_ADDRESS,&Data_Measurements);
			read_all_from_EEPROM(EEPROM_ADDRESS);
			ten_min = 0;
		}
		sei();
		
		/************************************************				DISPLAYS					*************************************************************/
		cli(); // Interuppt off, cannot display eeprom data while displaying measurements
			// ADC1 RAINDROPS DISPLAY
		tr_locate(1,1);
		tr_attr(1,YELLOW,BLACK);
		USART_PutS("RAIN_DROPS_ADC1 = ");
		USART_PutInt(meas1,10);
		USART_PutS("         \n");
			// VOLTAGE DISPLAY ADC1
		tr_locate(2,1);
		tr_pen_color(MAGENTA);
		USART_PutS("RAIN_DROPS_VOLTAGE = ");
		USART_PutInt(divmod1.quot,10);
		USART_PutS(".");
		USART_PutInt(divmod1.rem,10);
		tr_locate(2,27);
		USART_PutS("V\n");
		
			// ADC2 LIGHT INTENSITY DISPLAY
		tr_locate(3,1);
		tr_attr(1,CYAN,BLACK);
		USART_PutS("LIGHT_INTENSITY_ADC2 = ");
		USART_PutInt(meas2,10);
		USART_PutS("         \n");
			//VOLTAGE DISPLAY ADC2
		tr_locate(4,1);
		tr_pen_color(GREEN);
		USART_PutS("LIGHT_INTENSITY_VOLTAGE = ");
		USART_PutInt(divmod2.quot,10);
		USART_PutS(".");
		USART_PutInt(divmod2.rem,10);
		tr_locate(4,32);
		USART_PutS("V\n");
		
			//DHT11 DISPLAY
		if ((I_HUM + D_HUM + I_Temp + D_Temp) != CheckSum) 
			USART_PutS("Error");
		else
		{	
			tr_locate(5,1);
			tr_pen_color(BLUE);
			USART_PutS("Humidity = ");
			USART_PutInt(I_HUM,10);
			USART_PutS(".");
			USART_PutInt(D_HUM,10);
			USART_PutS("%\n");

			tr_locate(6,1);
			tr_pen_color(RED);
			USART_PutS("Temperature = ");
			USART_PutInt(I_Temp,10);
			USART_PutS(".");
			USART_PutInt(D_Temp,10);
			USART_PutC(176);
			USART_PutS("C\n\n");
		}	
		sei();
	}
}