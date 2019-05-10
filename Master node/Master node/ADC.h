/*
 * ADC.h
 *
 * Created: 07/04/2019 09:39:53 AM
 *  Author: Ahmed
 */ 


#ifndef ADC_H_
#define ADC_H_

#define a0 0
#define a1 1
#define a2 2
#define a3 3
#define a4 4
#define a5 5

#define A0 a0 
#define A1 a1 
#define A2 a2 
#define A3 a3 
#define A4 a4 
#define A5 a5 

void INIT_ADC();
int Analog_Read(uint8_t pin_num);



#endif /* ADC_H_ */