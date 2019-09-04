
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C.h"
#include "LCD.h"
#include "PWM.h"

#define ultra_S_ID 0x00
#define tempr_ID 0x40
#define on_off_ID 0x80
#define ID_MASK 0xc0
void task_LCD();
void task_PWM();

QueueHandle_t xQueueLCD;
QueueHandle_t xQueuePWM;

unsigned char isON = 1;

int main(void)
{
	xTaskCreate(task_LCD,"task_LCD",0x100,NULL,0x01,NULL);
	xTaskCreate(task_PWM,"task_PWM",0x100,NULL,0x01,NULL);
	xQueueLCD = xQueueCreate(0x0a,sizeof(unsigned char));
	xQueuePWM = xQueueCreate(0x0a,sizeof(unsigned char));
	i2c_initSlave(0x02); //i2c address = 0x02
	vTaskStartScheduler();
	sei();
	for (;;)
	{

	}

	return 0;
}

void task_PWM()
{
	PWM_start();
	const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
	unsigned char data;

	for(;;)
	{
		if( uxQueueMessagesWaiting( xQueuePWM ) != 0 )
		{
			xQueueReceive(xQueuePWM,&data,0);
			if(data = 0xff)
			{
				if (isON)
				{
					isON = 0;
					PWM0_setDutyCycle(0);
					PWM1_setDutyCycle(0);
				}
				else
				{
					isON = 1;
					PWM0_setDutyCycle(255);
					PWM1_setDutyCycle(255);
				}
				
			}
		}
		vTaskDelay( xDelay );
	}

}
void task_LCD()
{
	LCD_Init();
	const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
	unsigned char data;

	for(;;)
	{
		LCD_Clear();
		if( uxQueueMessagesWaiting( xQueueLCD ) != 0 )
		{
			xQueueReceive(xQueueLCD,&data,0);
			isON = 0;
			if((data & 0x80)) //from temperature sensor
			{
				LCD_String("Temp: ");
			}
			else //from ultra sonic sensor
			{
				LCD_String("US: ");
			}
			char buffer[10];
			LCD_String(itoa(data,buffer,10));
		}
		else if (isON)
		{
			LCD_String("MOTOR ON");
		}
		vTaskDelay( xDelay );
	}
}

ISR (TWI_vect)
{
	if(((TWSR & 0xF8))==0b01100000) //slaved addressed status
	{
		TWCR |= (1<<TWINT) ;
	}
	else if (((TWSR & 0xF8))==0b10000000) //slaved read status
	{
		unsigned char data = TWDR;
		xQueueSendToFront(xQueueLCD,&data,0);
		if ((data & ID_MASK) == ultra_S_ID)
		{
			xQueueSendToFront(xQueuePWM,&data,0);
		}
		TWCR |= (1<<TWINT) ;
	}
}
