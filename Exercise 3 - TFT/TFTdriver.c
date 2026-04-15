/************************************************************
File name: "TFTdriver.c"

Driver for "ITDB02 320 x 240 TFT display module, Version 2"
mounted at "ITDB02 Arduino Mega2560 Shield".
Display controller = ILI 9341.

Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

Connections:
DB15-DB8:   PORT A
DB7-DB0:    PORT C

RESETx:     PORT G, bit 0
CSx:        PORT G, bit 1
WRx:        PORT G, bit 2
RS (=D/Cx): PORT D, bit 7

Henning Hargaard
Modified Michael Alrøe
************************************************************/
#include <avr/io.h>

#define F_CPU 16000000
#include <util/delay.h>

#include <avr/cpufunc.h>  // _NOP()
#include "TFTdriver.h"

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW  PORTC

// Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define DC_PORT PORTD
#define DC_BIT  7  //"DC" signal is at the shield called RS
#define CS_PORT PORTG
#define CS_BIT  1
#define RST_PORT PORTG
#define RST_BIT 0

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
static void TFTWriteCommand(uint8_t command)
{
	// Data set-up (commands only use the lower byte of the data bus)
	DATA_PORT_LOW = command;

	// DC low
	DC_PORT &= ~(1 << DC_BIT);

	// CS low
	CS_PORT &= ~(1 << CS_BIT);

	// WR low
	WR_PORT &= ~(1 << WR_BIT);

	// twr0 > 15 ns
	_NOP();

	// WR high
	WR_PORT |= (1 << WR_BIT);
}

// ILI 9341 data sheet, page 238
static void TFTWriteData(uint16_t data)
{
	// Data set up
	DATA_PORT_LOW = data;
	DATA_PORT_HIGH = data >> 8;

	// DC high
	DC_PORT |= (1 << DC_BIT);

	// CS low
	CS_PORT &= ~(1 << CS_BIT);

	// WR low
	WR_PORT &= ~(1 << WR_BIT);

	// twr0 > 15 ns
	_NOP();

	// WR high
	WR_PORT |= (1 << WR_BIT);

}

// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

// Initializes (resets) the display
void TFTDisplayInit()
{
	// Control pins are outputs
	DDRG |= ((1 << WR_BIT) | (1 << CS_BIT) | (1 << RST_BIT));
	DDRD |= ((1 << DC_BIT));

	// Data pins are outputs
	DDRA |= 0b11111111;
	DDRC |= 0b11111111;

	// All control pins high;
	WR_PORT |= (1 << WR_BIT);
	CS_PORT |= (1 << CS_BIT);
	DC_PORT |= (1 << DC_BIT);

	// RST low
	RST_PORT &= ~(1 << RST_BIT);

	// Wait 500 ms
	_delay_ms(500);

	// RST high + Wait
	RST_PORT |= (1 << RST_BIT);
	_delay_ms(500);

	// Exit sleep mode
	TFTSleepOut();

	// Display on
	TFTDisplayOn();

	// Set bit BGR (scanning direction)
	TFTWriteCommand(0x36);
	TFTWriteData(0x28);

	// 16 bits (2 bytes) per pixel
	TFTWriteCommand(0x3A);
	TFTWriteData(0x55);

}

void TFTDisplayOff()
{
	TFTWriteCommand(0x28);
}

void TFTDisplayOn()
{
	TFTWriteCommand(0x29);
}

void TFTSleepOut()
{
	TFTWriteCommand(0x11);
}

void TFTMemoryAccessControl(uint8_t parameter)
{
	TFTWriteCommand(0x36);
	TFTWriteData(parameter);
}

void TFTInterfacePixelFormat(uint8_t parameter)
{
	TFTWriteCommand(0x3A);
	TFTWriteData(parameter);
}

void TFTMemoryWrite()
{
	TFTWriteCommand(0x2C);
}

// Red 0-31, Green 0-63, Blue 0-31
void TFTWritePixel(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	uint16_t color;

	// R-G-B = 5-6-5
	color = (Red & 0x1F) << 11;
	color |= (Green & 0x3F) << 5;
	color |= (Blue & 0x1F);

	TFTWriteData(color);
}

// Set Column Address (0-319), Start > End
void TFTSetColumnAddress(uint16_t Start, uint16_t End)
{
	TFTWriteCommand(0x2A);

	TFTWriteData(Start >> 8);
	TFTWriteData(Start & 0xFF);

	TFTWriteData(End >> 8);
	TFTWriteData(End & 0xFF);
}

// Set Page Address (0-239), Start > End
void TFTSetPageAddress(uint16_t Start, uint16_t End)
{
	TFTWriteCommand(0x2B);

	TFTWriteData(Start >> 8);
	TFTWriteData(Start & 0xFF);

	TFTWriteData(End >> 8);
	TFTWriteData(End & 0xFF);
}

// Fills rectangle with specified color
// (StartX,StartY) = Upper left corner. X horizontal (0-319) , Y vertical (0-239).
// Height (1-240) is vertical. Width (1-320) is horizontal.
// R-G-B = 5-6-5 bits.
void TFTFillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width,
	uint16_t Height, uint8_t Red, uint8_t Green, uint8_t Blue)
{
	uint32_t pixels;
	uint16_t color;

	uint16_t EndX = StartX + Width - 1;
	uint16_t EndY = StartY + Height - 1;

	// Set window
	TFTSetColumnAddress(StartX, EndX);
	TFTSetPageAddress(StartY, EndY);

	TFTMemoryWrite();

	// Convert color once
	color = (Red & 0x1F) << 11;
	color |= (Green & 0x3F) << 5;
	color |= (Blue & 0x1F);

	pixels = (uint32_t)Width * Height;

	while (pixels--)
	{
		TFTWriteData(color);
	}
}