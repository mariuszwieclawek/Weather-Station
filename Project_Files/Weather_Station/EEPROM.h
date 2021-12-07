/*
 * EEPROM.h
 *
 * Created: 23.11.2021 15:01:29
 *  Author: Mariusz
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

typedef struct{
	char I_Humidity;
	char D_Humidity;
	char I_Temperature;
	char D_Temperature;
	char I_sensor1;
	char D_sensor1;
	char I_sensor2;
	char D_sensor2;
}DATA;

DATA Data_Measurements;

void EEPROM_write(unsigned int uiAddress, int8_t ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
void write_all_to_EEPROM(unsigned int uiAddress, DATA * pomiary );
void read_all_from_EEPROM( unsigned int uiAddress );

#endif /* EEPROM_H_ */