
#ifndef DHT11_H_
#define DHT11_H_

void DHT_Request();				/* Microcontroller send start pulse/request */
void DHT_Response();				/* receive response from DHT11 */
uint8_t DHT_Receive_data();			/* receive data */

#endif /* DHT11_H_ */