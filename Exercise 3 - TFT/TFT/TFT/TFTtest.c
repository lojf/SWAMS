
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "UARTdriver.h"
#include "ADXdriver.h"


#define STILL_TIME_MS   3000   // skal være stille i 3 sekunder
#define CHECK_INTERVAL  1000    // tjek hvert 1000ms

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
	adxl_init();

	char buffer[6];
	FormatTime(buffer);
	DrawTime(buffer);

	uint16_t still_count = 0;
	uint8_t time_requested = 0;

	while (1)
	{
		if (uart1_available())
		{
			HandleUARTCommand();
			time_requested = 0;
		}
		
		if (adxl_is_still())
		{
			still_count++;
			uart0_send_string("Stille! Count: ");
			// send selve tallet
			char countStr[4];
			countStr[0] = (still_count / 100) + '0';
			countStr[1] = (still_count / 10 % 10) + '0';
			countStr[2] = (still_count % 10) + '0';
			countStr[3] = '\0';
			uart0_send_string(countStr);
			uart0_send_string("\r\n");

			if (still_count >= (STILL_TIME_MS / CHECK_INTERVAL) && !time_requested)
			{
				uart0_send_string("Sender GET_TIME!\r\n");
				uart1_send_string("GET_TIME\n");
				time_requested = 1;
			}
		}
		else
		{
			uart0_send_string("Bevaegelse registreret\r\n");
			still_count = 0;
			time_requested = 0;
		}
		
		_delay_ms(CHECK_INTERVAL);
	}
	
	
	
	
}
