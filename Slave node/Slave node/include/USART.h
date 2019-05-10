/*
 * USART.h
 *
 * Created: 10/01/2017 05:51:14 م
 *  Author: D.Randa
 */ 


#ifndef USART_H_
#define USART_H_


void USART_init(void);
void USART_sendstr( unsigned char* str);
void USART_send( unsigned char data);
unsigned char USART_receive(void);

#endif /* USART_H_ */