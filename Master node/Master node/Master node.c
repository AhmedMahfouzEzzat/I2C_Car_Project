#define F_CPU 16000000UL
#define TrackSize 5
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "USART.h"
#include "wiring.h"
#include "queue.h"
#include "I2C.h"
#include "ADC.h"
#include "hc-sr04.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#define Master
#define ultra_S_ID 0x00
#define tempr_ID 0x40
#define on_off_ID 0x80 
#define ID_MASK 0xc0

static void ON_OFF_ISR();
volatile unsigned char  flag = 0;
/* The tasks to be created.  Two instances are created of the sender task while
only a single instance is created of the receiver task. */
void task_I2C();
//void task_Generator();//Just for testing
void task_ADC();
void ultrsonic_task();
void ON_OFF_task();

#define LONG_TIME 0xffff
#define TICKS_TO_WAIT    10

//-----------------------------------------------------------
TaskHandle_t i2c_Handle;
TaskHandle_t adc_Handle;
TaskHandle_t ultr_Handle;
TaskHandle_t on_off_Handle;

//SemaphoreHandle_t sem;
QueueHandle_t xQueue;
int main(void)
{ 
	cli(); 
	// initialization 
	USART_init();
	//interrupt mode (trigger on low input, any change, rising edge, or falling edge)
	attachInterrupt(0, ON_OFF_ISR, 3); //faling 
	//sem =  xSemaphoreCreateBinary();
	xTaskCreate(task_I2C,"task_I2C",0x100,NULL,0x04,i2c_Handle);
	//xTaskCreate(task_Generator,"task_Generator",0x100,NULL,0x02,NULL);
	xTaskCreate(task_ADC,"task_ADC",0x100,NULL,0x01,adc_Handle);
	xTaskCreate(ultrsonic_task,"task_ultra",0x100,NULL,0x00,ultr_Handle);
	xTaskCreate(ON_OFF_task,"task_on_off",0x100,NULL,0x03,on_off_Handle);
	sei();
	vTaskStartScheduler();
	 
	for( ;; );
	return 0;
}

void ultrsonic_task()
{
	init_ultrasonic_sensor();
	const TickType_t xDelay = 50 / portTICK_PERIOD_MS;
	int dist = 0;
	for (;;)
	{
		dist=read_dist();
		USART_send(dist);
		if(dist < 20)
		{
			dist &= ~ ID_MASK ;
			dist |= ultra_S_ID ; //add message id in last 2 bit 
			xQueueSendToFront(xQueue,&dist,TICKS_TO_WAIT);
		}
		vTaskDelay( xDelay );
	}

}

void task_ADC()
{
	INIT_ADC();
	const TickType_t xDelay = 50 / portTICK_PERIOD_MS;  //time to wait
	for (;;)
	{
		unsigned char data = Analog_Read(0);
		data =(data*500)/1023;
		if(data>30)
		{
			data &= ~ ID_MASK ;
			data |= tempr_ID ;  //add message id in last 2 bit  
			xQueueSendToFront(xQueue,&data,TICKS_TO_WAIT);
		}
		
		vTaskDelay( xDelay );
	}
}


void task_I2C()
{
	unsigned char data;
	_delay_ms(500);//wait slave to initialize
	i2c_init();
	i2c_start();
	i2c_write(0x02); //address
	const TickType_t xDelay = 40 / portTICK_PERIOD_MS;
	for (;;)
	{
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			xQueueReceive(xQueue,&data,TICKS_TO_WAIT);
			i2c_write(data); //data
		}
		
		vTaskDelay( xDelay );
	}
	i2c_stop();
}
void ON_OFF_task()
{
	if(!flag)
	{
		vTaskSuspend(ultr_Handle);
		vTaskSuspend(adc_Handle);
	}
	else
	{
		vTaskResume(ultr_Handle);
		vTaskResume(adc_Handle);
	}
	vTaskSuspend(NULL);
}

void ON_OFF_ISR()
{
	flag = !flag;
	flag |= on_off_ID;
	xQueueSendToFrontFromISR(xQueue,&flag,TICKS_TO_WAIT);
	xTaskResumeFromISR(on_off_Handle);
}