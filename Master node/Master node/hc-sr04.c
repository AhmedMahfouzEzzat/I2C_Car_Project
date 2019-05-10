#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint16_t rising, falling;
int counts;
float dist;
uint16_t us_per_count;


ISR(TIMER1_COMPB_vect){

    //Generate a 12us pulse to trigger the HR-SR04
    PORTB ^= ( 1 << PORTB1);
    _delay_us(12);
    PORTB ^= ( 1 << PORTB1);
}

ISR(TIMER1_CAPT_vect){

    if(TCCR1B & (1 << ICES1)){

        TCCR1B &= ~( 1 << ICES1);
        rising = ICR1;
    }
    else{
        TCCR1B |= (1 << ICES1);
        falling = ICR1;
        counts = falling - rising;
        dist = (float)us_per_count * counts * 10 / 580;
    }
}


void init_ultrasonic_sensor(){

    //PORTB2 as output (Arduino PIN 10)
    DDRB |= (1 << PORTB1);  

    //****TIMER1 INIT****//

    //noice reduction enable
    TCCR1B |= (1<<ICNC1); 

    //Prescale to 64
    TCCR1B |= (1<<CS10) | (1<<CS11); 

    //select CTC mode
    TCCR1B |= (1<<WGM12); 

    //read Int. at: rising edge, falling edge
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1B);

    //read Int. at rising edge
	TCCR1B |= (1<<ICES1);

    //calculate TOP (70ms runtime for one cycle): 16MHz/64 = 25000 count/sec. = 25000/1000 = 2500 count/us / 100*70 = 17500 count/70ms
    OCR1B = 17500;

    //CPU-speed / Prescale = cycles/sec. 1sec/ freq./sec. = 4 (t= 4us/cycle)
    us_per_count = 4;

    //activate global interrupt
    sei();

}

float read_dist(){
    return dist;
}