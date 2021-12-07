/*
 * DHT11.h
 *
 * Created: 16.11.2021 17:58:16
 *  Author: Mariusz
 */ 


#ifndef DHT11_H_
#define DHT11_H_

void DHT_Request();				/* Microcontroller send start pulse/request */
void DHT_Response();				/* receive response from DHT11 */
uint8_t DHT_Receive_data();			/* receive data */

#endif /* DHT11_H_ */