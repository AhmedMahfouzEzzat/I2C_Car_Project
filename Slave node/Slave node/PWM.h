/*
 * pwm.h
 *
 * Created: 4/8/2019 2:51:49 PM
 *  Author: Hussam
 */ 


#ifndef PWM_H_
#define PWM_H_


void PWM_start();
void PWM0_setDutyCycle(unsigned char dutyCycle);
void PWM1_setDutyCycle(unsigned char dutyCycle);
void PWM0_ClockWise();
void PWM0_AntiClockWise();
void PWM1_ClockWise();
void PWM1_AntiClockWise();


#endif /* PWM_H_ */