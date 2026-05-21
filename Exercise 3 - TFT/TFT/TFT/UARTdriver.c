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

// handle uart command
void HandleUARTCommand(char c)
{
	switch (c)
	{
		case '1': IncrementTime(15); break;
		case '2': IncrementTime(30); break;
		case '3': IncrementTime(60); break;
		default:  return;
	}

	char buffer[6];
	FormatTime(buffer);
	DrawTime(buffer);
}