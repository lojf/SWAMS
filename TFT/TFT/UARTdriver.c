#define F_CPU 16000000UL
#include "UARTdriver.h"
#include "TFTdriver.h"
#include <avr/io.h>
#include <util/delay.h>


// --- UART0 - debug til PC ---
void uart0_init(void)
{
	UBRR0H = (UBRR_VAL >> 8);
	UBRR0L = (UBRR_VAL & 0xFF);
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart0_send_char(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart0_send_string(const char* str)
{
	while (*str)
	{
		uart0_send_char(*str++);
	}
}

// --- UART1 - modtager fra ESP32 ---
void uart1_init(void)
{
	UBRR1H = (UBRR_VAL >> 8);
	UBRR1L = (UBRR_VAL & 0xFF);
	UCSR1B = (1 << TXEN1) | (1 << RXEN1);
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

uint8_t uart1_available(void)
{
	return (UCSR1A & (1 << RXC1));
}

char uart1_read_char(void)
{
	while (!uart1_available());
	return UDR1;
}

void uart1_send_char(char c)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = c;
}

void uart1_send_string(const char* str)
{
	while(*str){
		uart1_send_char(*str);
		str++;
	}
}
// handle uart command
void HandleUARTCommand(void)
{
	 char buf[6];
	 char c;

	 // Skip \r, \n and non-numbers. (because println() works best)
	 do {
		 c = uart1_read_char();
	 } while (c < '0' || c > '9');

	 buf[0] = c;
	 for (int i = 1; i < 5; i++)
	 {
		 buf[i] = uart1_read_char();
	 }
	 buf[5] = '\0';

	// validate there is a comma, maybe not so necessary but nice to have
	if (buf[2] != ':') return;

	// parse time
	int h = (buf[0] - '0') * 10 + (buf[1] - '0');
	int m = (buf[3] - '0') * 10 + (buf[4] - '0');

	// Validate values, also not needed because it is done on website already, but nice for re-use
	if (h < 0 || h > 23) return;
	if (m < 0 || m > 59) return;

	// set the time and use it for drawing the time on the pskive
	SetTime(h, m);

	char buffer[6];
	FormatTime(buffer);
	DrawTime(buffer);
	
}