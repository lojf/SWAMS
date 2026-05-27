/*
 * ADXdriver.c
 *
 *  Has the purpose of using I2C to communicate with accelerometer
 *	Lots of bits will be set to establish communication
 *	a buffer will be used for the data byte
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ADXdriver.h"
#include "I2Cdriver.h"

#define ADXL_ADDR 0x53  // default I2C adresse for ADXL343

static void adxl_write_reg(uint8_t reg, uint8_t value)
{
	i2c_start();
	i2c_write(ADXL_ADDR << 1);  // skriv
	i2c_write(reg);
	i2c_write(value);
	i2c_stop();
}

static int8_t adxl_read_reg(uint8_t reg)
{
	i2c_start();
	i2c_write(ADXL_ADDR << 1);  // skriv registeradresse
	i2c_write(reg);
	i2c_start();                 // repeated start
	i2c_write((ADXL_ADDR << 1) | 0x01);  // læs
	uint8_t val = i2c_read_nack();
	i2c_stop();
	return (int8_t)val;
}

void adxl_init(void)
{
	i2c_init();
	_delay_ms(10);
	adxl_write_reg(0x2D, 0x08);  // POWER_CTL: measurement mode
	adxl_write_reg(0x31, 0x00);  // DATA_FORMAT: +/- 2g, 10 bit
}

uint8_t adxl_is_still(void)
{
	// Læs X, Y, Z acceleration (register 0x32-0x37)
	// Vi bruger kun high byte (0x33, 0x35, 0x37) for enkelhedens skyld
	int8_t x = adxl_read_reg(0x33);
	int8_t y = adxl_read_reg(0x35);
	int8_t z = adxl_read_reg(0x37);

	// Fjern tyngdekraft fra Z (ca. 64 LSB ved 2g range = 1g)
	z -= 64;

	// Threshold: hvis alle akser er under ~0.15g anses den som stille
	// Ved 2g range og 10 bit = ca. 256 LSB per g ? high byte = 1 LSB per ~0.015g
	// Threshold på 10 svarer til ca. 0.15g
	#define STILL_THRESHOLD 10

	if (x < -STILL_THRESHOLD || x > STILL_THRESHOLD) return 0;
	if (y < -STILL_THRESHOLD || y > STILL_THRESHOLD) return 0;
	if (z < -STILL_THRESHOLD || z > STILL_THRESHOLD) return 0;

	return 1;
}