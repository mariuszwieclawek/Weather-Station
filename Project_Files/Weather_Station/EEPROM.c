#include <avr/io.h>
#include <avr/interrupt.h>

#include "terminal.h"
#include "usart.h"
#include "EEPROM.h"

void EEPROM_write(unsigned int uiAddress, int8_t ucData)
{
	cli();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);  //When the write access time has elapsed, the EEPE bit is cleared by hardware.
	sei();
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	cli();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
	sei();
}

void write_all_to_EEPROM(unsigned int uiAddress, DATA * measurements )
{
	EEPROM_write(uiAddress*8,measurements->I_Humidity);
	EEPROM_write(uiAddress*8+1,measurements->D_Humidity);
	EEPROM_write(uiAddress*8+2,measurements->I_Temperature);
	EEPROM_write(uiAddress*8+3,measurements->D_Temperature);
	EEPROM_write(uiAddress*8+4,measurements->I_sensor1);
	EEPROM_write(uiAddress*8+5,measurements->D_sensor1);
	EEPROM_write(uiAddress*8+6,measurements->I_sensor2);
	EEPROM_write(uiAddress*8+7,measurements->D_sensor2);
}

void read_all_from_EEPROM( unsigned int uiAddress )
{
	static char n = 1;
	tr_locate(9,1);
	tr_attr(1,WHITE,BLACK);
	USART_PutS("==============================READ FROM EEPROM==============================\n");
	tr_locate(10,1);
	USART_PutS("	                 Measurements every 10 minutes!");
	tr_locate(11,1);
	USART_PutS("============================================================================\n");
	tr_locate(12,1);
	USART_PutS("n");
	tr_locate(12+n,1);
	USART_PutInt(n,10);
	tr_locate(12,5);
	tr_attr(1,BLUE,BLACK);
	USART_PutS("Humidity [%]");
	tr_locate(12+n,5);
	USART_PutInt(EEPROM_read(uiAddress*8),10);
	USART_PutC('.');
	USART_PutInt(EEPROM_read(uiAddress*8+1),10);
	tr_locate(12,20);
	tr_attr(1,RED,BLACK);
	USART_PutS("Temperature [°]");
	tr_locate(12+n,20);
	USART_PutInt(EEPROM_read(uiAddress*8+2),10);
	USART_PutC('.');
	USART_PutInt(EEPROM_read(uiAddress*8+3),10);
	tr_locate(12,40);
	tr_attr(1,MAGENTA,BLACK);
	USART_PutS("RAIN_VOLTAGE [V]");
	tr_locate(12+n,40);
	USART_PutInt(EEPROM_read(uiAddress*8+4),10);
	USART_PutS(".");
	USART_PutInt(EEPROM_read(uiAddress*8+5),10);
	tr_locate(12,60);
	tr_attr(1,GREEN,BLACK);
	USART_PutS("LIGHT_VOLTAGE [V]");
	tr_locate(12+n,60);
	USART_PutInt(EEPROM_read(uiAddress*8+6),10);
	USART_PutS(".");
	USART_PutInt(EEPROM_read(uiAddress*8+7),10);
	
	n++;
	
	if(n == 10)
	{
		n = 1;
		tr_cls(1);
	}
	
}