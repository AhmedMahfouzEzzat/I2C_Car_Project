/*
 * I2C.c
 *
 * Created: 4/28/2019 5:12:47 PM
 *  Author: Hussam
 */ 

 #include "I2C.h"
 #include <avr/io.h>
 void i2c_initSlave(unsigned char slaveAddress)
 {
	 TWCR = 0x04; //enable module
	 TWAR = slaveAddress;
	 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // init module
 }

 void i2c_send(unsigned char data)
 {
	 TWDR = data;
	 TWCR = (1<<TWINT) |(1<<TWEN);
	 while ((TWCR & (1<<TWINT) ) == 0);
 }

 void i2c_listen()
 {
	 while((TWCR&(1<<TWINT))==0);
 }


 void i2c_write(unsigned char data)
 {
	 TWDR = data ;
	 TWCR = (1<<TWINT) | (1<<TWEN);
	 while ((TWCR & (1<<TWINT))==0);
 }

 unsigned char i2c_read()
 {
	 TWCR = (1<<TWINT) | (1<<TWEN);
	 while ((TWCR & (1<<TWINT))==0);
	 return TWDR;
 }

 void i2c_start(void)
 {
	 TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	 while ((TWCR & (1<<TWINT)));
	 while((TWSR & 0xF8)!= 0x08); //wait for ack
 }

 void i2c_stop()
 {
	 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
 }

 void i2c_init(void)
 {
	 TWSR = 0x00; //set prescalar to zero
	 TWBR = 0x47; //SCL frequency is 50k for XTAL = 8M
	 TWCR = 0x04; //enable the TWI module
 }