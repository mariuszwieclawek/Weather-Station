
#ifndef USART_H_
#define USART_H_

void USART_Init( unsigned int ubrr); // Initialization UART
void USART_PutC( char data ); // function that sends a single character
void USART_PutS( char * s); // function that sends a string
void USART_PutInt(uint16_t number, uint8_t radix); // send the number and specify what format we want (bin/dec/hex)
void USART_PutS_P(const char *s);	// sends string from RAM to UART
unsigned char USART_Receive( void ); // get and return received data from buffer 


#endif /* USART_H_ */