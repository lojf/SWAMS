
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "UARTdriver.h"


int main(void)
{	
	uart0_init();
	uart1_init();
	TFTDisplayInit();

	char buffer[6];
	FormatTime(buffer);
	DrawTime(buffer);

	while (1)
	{
		if (uart1_available())
		{
			HandleUARTCommand();
		}
	}
	
}
