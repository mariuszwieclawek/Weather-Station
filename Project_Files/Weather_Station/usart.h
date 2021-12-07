/*
 * usart.h
 *
 * Created: 16.11.2021 11:32:30
 *  Author: Mariusz
 */ 


#ifndef USART_H_
#define USART_H_

void USART_Init( unsigned int ubrr); // Inicjalizacja UART
void USART_PutC( char data ); // Wysylamy pojedynczy znak
void USART_PutS( char * s); // Wysylamy string, ciag znakow
void USART_PutInt(uint16_t number, uint8_t radix); // Wysylamy liczbe int, radix to w jakim formacie wyswietlamy liczbe 2-dwojkowy 10-dziesiatkowy 16-szesntastkowy
void USART_PutS_P(const char *s);	// wysy³a ³añcuch z pamiêci RAM na UART
unsigned char USART_Receive( void ); // Odbiera wyslane dane 


#endif /* USART_H_ */