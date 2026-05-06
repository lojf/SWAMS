/*
* TFTtest.c
* Test program for the TFTdriver
*
* Henning Hargaard
* Modified Michael Alrøe
*/

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"

void DrawDannebrog()
{
	// All pixels white (background)
	TFTFillRectangle(0, 0, 320, 240, 31, 63, 31);
	// Draw red parts of danish flag
	TFTFillRectangle(0, 140, 100, 100, 31, 0, 0);
	TFTFillRectangle(0, 0, 100, 100, 31, 0, 0);
	TFTFillRectangle(140, 0, 320 - 140, 100, 31, 0, 0);
	TFTFillRectangle(140, 140, 320 - 140, 100, 31, 0, 0);

}

int main(void)
{
	// Initialize the display
	TFTDisplayInit();

	DrawDannebrog();
	while (1)
	{
		TFTDisplayOn();
		_delay_ms(2000);
		TFTDisplayOff();
		_delay_ms(1000);
	}
}
