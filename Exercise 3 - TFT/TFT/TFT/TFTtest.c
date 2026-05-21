
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "UARTdriver.h"


int main(void)
{
	
	 // TEST 1
	 //TFTDisplayInit();

	 // ?? CHANGE THIS LINE TO TEST DIFFERENT TIMES
	 /*
	 char time[] = "11:30";

	 DrawTime(time);

	 while (1)
	 {
	 }
	 */
	//TEST 2
	//TFTDisplayInit();
//
	//char timeStr[6];
//
	//while (1)
	//{
		//FormatTime(timeStr);
		//DrawTime(timeStr);
//
		//_delay_ms(5000);   // wait 5 seconds
//
		//IncrementTime();
	//}
	
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
			char modtaget = uart1_read_char();
			uart0_send_char(modtaget);
			HandleUARTCommand(modtaget);
		}
	}
	
	
	
	
}
