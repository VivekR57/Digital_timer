#ifndef E_EPROM_H
#define E_EPROM_H

#define READ		0xA1
#define WRITE		0xA0

void write_eeprom(unsigned char address, unsigned char data);
unsigned char read_eeprom(unsigned char address);

#endif