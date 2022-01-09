
#ifndef TERMINAL_H
#define TERMINAL_H

// character attributes
#define RESET		0
#define BOLD 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

// font or background colors
#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7


void tr_cls(uint8_t cur_onoff);						// erase screen, cursor on / off
void tr_cursor_hide( uint8_t hide );				// cursor show=0 / hide=1
void tr_locate( uint8_t y, uint8_t x );				// set row and column
void tr_pen_color( uint8_t cl );					// set the font color
void tr_brush_color( uint8_t cl );					// set the background color
void tr_attr( uint8_t atr, uint8_t fg, uint8_t bg );// set attribute: character, font and background color

void fill_line( char ascii, uint8_t cnt );


#endif
