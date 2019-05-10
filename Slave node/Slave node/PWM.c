/*
 * PWM.c
 *
 * Created: 4/8/2019 2:51:21 PM
 *  Author: Hussam
 */ 
#include <avr/io.h>
/*
D0,D1 pwm0 direction -> output on D6
D4,D7 pwm1 direction -> output on D7
*/


void PWM0_setDutyCycle(unsigned char dutyCycle)
{
	OCR0A = dutyCycle;
}

void PWM1_setDutyCycle(unsigned char dutyCycle)
{
	OCR0B = dutyCycle;
}
void PWM0_ClockWise()
{
	PORTD |=  (1 << 0) ;
	PORTD &=  ~(1 << 1) ;
}

void PWM0_AntiClockWise()
{
	PORTD |=  (1 << 1) ;
	PORTD &=  ~(1 << 0) ;
}

void PWM1_ClockWise()
{
	PORTD |=  (1 << 4) ;
	PORTD &=  ~(1 << 7) ;
}

void PWM1_AntiClockWise()
{
	PORTD |=  (1 << 7) ;
	PORTD &=  ~(1 << 4) ;
}
void PWM_start()
{
	DDRD |= (1 << 6) | (1 << 1) | (1 << 0) | (1 << 7) | (1 << 4) | (1 << 5) ;

	TCCR0A |= (1 << COM0A1) | (1<<COM0B1);
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << CS01);

	PWM0_setDutyCycle(255);
	PWM1_setDutyCycle(255);
	PWM0_ClockWise();
	PWM1_AntiClockWise();
}

