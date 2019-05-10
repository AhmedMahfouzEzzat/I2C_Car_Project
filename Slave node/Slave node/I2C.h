/*
 * I2C.h
 *
 * Created: 4/28/2019 5:07:46 PM
 *  Author: Hussam
 */ 
void i2c_initSlave(unsigned char slaveAddress);

void i2c_send(unsigned char data);

void i2c_listen();

unsigned char i2c_read();

void i2c_write(unsigned char data);

void i2c_start();

void i2c_stop();

void i2c_init();

