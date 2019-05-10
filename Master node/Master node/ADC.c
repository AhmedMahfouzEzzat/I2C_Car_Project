/*
 * ADC.c
 *
 * Created: 07/04/2019 09:39:41 AM
 *  Author: Ahmed
 */ 
#include <avr/io.h>
void INIT_ADC()
{
	ADMUX|=(1<<REFS0); 
	ADCSRA|=(1<<ADEN); //enable ADC
	ADCSRA|=(7<<ADPS0);//clock Division Factor
}
	
int Analog_Read(uint8_t pin_num)
{
	ADMUX|= pin_num;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;	
}