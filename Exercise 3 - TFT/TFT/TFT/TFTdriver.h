/************************************************************
File name: "TFTdriver.c"

DB15-DB8:   PORT A
DB7-DB0:    PORT C

RESETx:     PORT G, bit 0
CSx:        PORT G, bit 1
WRx:        PORT G, bit 2
RS (=D/Cx): PORT D, bit 7
************************************************************/
#pragma once

void TFTDisplayInit();
void TFTDisplayOff();
void TFTDisplayOn();
void TFTSleepOut();
void TFTMemoryAccessControl(uint8_t parameter);
void TFTInterfacePixelFormat(uint8_t parameter);
void TFTWritePixel(uint8_t Red, uint8_t Blue, uint8_t Green);
void TFTSetColumnAddress(uint16_t Start, uint16_t End);
void TFTSetPageAddress(uint16_t Start, uint16_t End);
void SetTime(int h, int m);
void TFTMemoryWrite();
void TFTFillRectangle(uint16_t StartX, uint16_t StartY, uint16_t Width,
	 uint16_t Height, uint8_t Red, uint8_t Green, uint8_t Blue);