/*
 * I2Cdriver.c
 *
 * Created: 26-05-2026 10:39:17
 *  Author: micke
 */ 
#define F_CPU 16000000UL
#include "I2Cdriver.h"

// I2C helpers
void i2c_init(void)
{
	TWSR = 0x00;                        // prescaler = 1
	TWBR = ((F_CPU / 100000UL) - 16) / 2;  // 100 kHz I2C
}

void i2c_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}