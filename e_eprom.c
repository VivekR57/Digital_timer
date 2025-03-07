
#include "main.h"

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */



void write_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int i=3000;i--;);
}

unsigned char read_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(READ);
	data = i2c_read();
	i2c_stop();

	return data;
}