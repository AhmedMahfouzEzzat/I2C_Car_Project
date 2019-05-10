/*
 * Serial_send.c
 *
 * Created: 04/12/2016 09:36:31 م
 *  Author: D.Randa
 */ 

#include "USART.h"
#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 9600        //The baud rate that we want to use
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths



void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<UCSZ02);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}


void USART_sendstr( unsigned char* str){
	int i =0;
	
	while (str[i] != 0x00)
	{
		USART_send(str[i]);
		i++;
	}
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}