/*
 * usart.c
 *
 * Created: 16.11.2021 11:31:16
 *  Author: Mariusz
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#include "usart.h"

void USART_Init( unsigned int ubrr) // Initialization UART
{
	UBRR0H = (unsigned char)(ubrr>>8); //Set baud rate
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter 
	
	UCSR0C = (3<<UCSZ00); // Set frame format: 8data, 1stop bit 
}

void USART_PutC( char data ) // function that sends a single character
{
	while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer
	UDR0 = data; // Put data into buffer, sends the data
}

void USART_PutS( char * s) // function that sends a string
{
	while( *s ) USART_PutC( *s++ );
}

void USART_PutInt(uint16_t number, uint8_t radix) // send the number and specify what format we want (bin/dec/hex)
{
	char buf[17];
	itoa(number,buf,radix); //bin/dec/hex
	USART_PutS(buf);
}

void USART_PutS_P(const char *s)	//sends string from RAM to UART
{
	register char c;
	while((c = pgm_read_byte(s++) )) USART_PutC(c);
}

unsigned char USART_Receive( void ) // get and return received data from buffer 
{
	while ( !(UCSR0A & (1<<RXC0)) ); 	// Wait for data to be received
	return UDR0;
}