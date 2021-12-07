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

void USART_Init( unsigned int ubrr) // Inicjalizacja UART
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}

void USART_PutC( char data ) // Wysylamy pojedynczy znak
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_PutS( char * s) // Wysylamy string, ciag znakow
{
	while( *s ) USART_PutC( *s++ );
}

void USART_PutInt(uint16_t number, uint8_t radix) // Wysylamy liczbe int, radix to w jakim formacie wyswietlamy liczbe 2-dwojkowy 10-dziesiatkowy 16-szesntastkowy
{
	char buf[17];
	itoa(number,buf,radix); //funkcja integer to ascii, jak za male wartosci to zmienic int na log i funkcja ltoa
	USART_PutS(buf);
}

void USART_PutS_P(const char *s)	// wysy³a ³añcuch z pamiêci RAM na UART
{
	register char c;
	while((c = pgm_read_byte(s++) )) USART_PutC(c);
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}