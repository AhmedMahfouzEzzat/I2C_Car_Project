#define F_CPU 16000000UL
#define TrackSize 5
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "USART.h"
#include "wiring.h"
#include "queue.h"
//#include "I2C.h"
//#include "ADC.h"
//#include "hc-sr04.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

static void vTimerISR();

/* The tasks to be created.  Two instances are created of the sender task while
only a single instance is created of the receiver task. */
void task_I2C();
void task_Generator();//Just for testing
void task_ADC();

#define LONG_TIME 0xffff
#define TICKS_TO_WAIT    10

//-----------------------------------------------------------
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

SemaphoreHandle_t sem;
QueueHandle_t xQueue;
int main(void)
{ 
	cli(); 
	// initialization 
	xTaskCreate(task_I2C,"task_I2C",0x100,NULL,0x01,NULL);
	xTaskCreate(task_Generator,"task_Generator",0x100,NULL,0x02,NULL);
	xTaskCreate(task_ADC,"task_ADC",0x100,NULL,0x02,NULL);
	sei();
	vTaskStartScheduler();
	 
	for( ;; );
	return 0;
}


void task_ADC()
{
	INIT_ADC();
	const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
	for (;;)
	{
		unsigned char data = Analog_Read(0);
		data =(data*500)/1023;
		data |= 0x80;
		xQueueSendToFront(xQueue,&data,0);
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
			xQueueReceive(xQueue,&data,0);
			i2c_write(data); //data
		}
		vTaskDelay( xDelay );
	}
	i2c_stop();
}

void task_Generator()
{
	unsigned char counter = 'a';
	const TickType_t xDelay = 90 / portTICK_PERIOD_MS;
	for (;;)
	{
		xQueueSendToFront(xQueue,&counter,0);
		counter = (counter == 'z' ? 0 : counter+1);
		vTaskDelay( xDelay );
	}
}