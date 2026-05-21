#pragma once
#include <avr/io.h>   // <-- giver uint8_t, uint16_t osv.


#define BAUD 9600
#define UBRR_VAL (F_CPU / 16 / BAUD - 1)  // F_CPU sættes af compiler-flags

// UART0 - kun til debug (may be unødvendig)
void uart0_init(void);
void uart0_send_char(char c);
void uart0_send_string(const char* str);

// UART 1 til modtagelse af esp
void uart1_init(void);
uint8_t uart1_available(void);
char uart1_read_char(void);

//handle the time updating command
void HandleUARTCommand(char c);


