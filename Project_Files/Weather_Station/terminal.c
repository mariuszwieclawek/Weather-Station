
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "usart.h"
#include "terminal.h"

const char UCLS[] PROGMEM = { "\x1b""[2J" };

const char UHOME[] PROGMEM = { "\x1b""[;H" };

const char UCUR_HIDE[] PROGMEM = { "\x1b""[?25l" };		// cursor on
const char UCUR_SHOW[] PROGMEM = { "\x1b""[?25h" };		// cursor off

const char U_ATTR_OFF[] PROGMEM = { "\x1b""[m" };



void tr_cursor_hide( uint8_t hide ) {
	if(hide) USART_PutS_P( UCUR_HIDE );
	else USART_PutS_P( UCUR_SHOW );
}


void tr_cls(uint8_t cur_onoff) {

	USART_PutS_P( U_ATTR_OFF );
	tr_cursor_hide(cur_onoff);
	USART_PutS_P( UCLS );

	USART_PutS_P( UHOME );

}


void fill_line( char ascii, uint8_t cnt ) {
	for(uint8_t i=0; i<cnt; i++) USART_PutC( ascii );
}


void tr_attr( uint8_t atr, uint8_t fg, uint8_t bg ) {
	USART_PutC( 0x1b );		// <ESC>[0;32;44m
	USART_PutC( '[' );
	USART_PutC( atr+'0' );
	USART_PutC( ';' );
	USART_PutC( '3' );
	USART_PutC( fg+'0' );
	USART_PutC( ';' );
	USART_PutC( '4' );
	USART_PutC( bg+'0' );
	USART_PutC( 'm' );
}


void tr_pen_color( uint8_t cl ) {
	USART_PutC( 0x1b );		// <ESC>[34m
	USART_PutC( '[' );
	USART_PutC( '3' );
	USART_PutC( cl+'0' );
	USART_PutC( 'm' );
}

void tr_brush_color( uint8_t cl ) {
	USART_PutC( 0x1b );		// <ESC>[44m
	USART_PutC( '[' );
	USART_PutC( '4' );
	USART_PutC( cl+'0' );
	USART_PutC( 'm' );
}


void tr_locate( uint8_t y, uint8_t x ) {

	USART_PutC( 0x1b );	// <ESC>[y;xH
	USART_PutC( '[' );
	USART_PutInt( y, 10 );
	USART_PutC( ';' );
	USART_PutInt( x, 10 );
	USART_PutC( 'H' );
}


