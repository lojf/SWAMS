/*
 * I2Cdriver.h
 *
 * Created: 26-05-2026 10:39:04
 *  Author: mickey
 */ 

#ifndef I2CDRIVER_H
#define I2CDRIVER_H

#include <avr/io.h>

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

#endif