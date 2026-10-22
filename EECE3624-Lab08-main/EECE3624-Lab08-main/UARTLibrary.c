/*****************************************************************************
 *	    File: USARTLibrary.c
 *  Lab Name: (Needed for Lab07 and Lab08)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: 
 *  Mod Date:
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUSART1Library.a and USART1Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the USART1Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default USART1 port
 * on the ReadyAVR (so JP2 must have jumpers on PD3 and PD2).
 */

#include <avr/io.h>
#include "UARTLibrary.h"

// Use USART1_init to initialize the USART hardware in the AVR processor.
// The function takes the following arguments:
//    - desired baud rate (standard values from 2400 to 115200),
//    - number of databits (5-8),
//    - number of stop bits (1,2),
//    - parity (0=no parity, 1=odd parity, 2=even parity).
// No error checking is performed and the function does not return a value.
//
// For example,
//     usart1_init(9600,8,1,0);
// sets the baudrate to 9600, with 8 data bits, 1 stop bit, and no parity
void uart_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity) {
    /* your code here */
	// set baud rate (via UBRR0 high and low registers)
	
	// convert baudrate into UBRR register value
    unsigned long UBRR_VAL = (F_OSC / (16 * baudrate) - 1);
	// store HIGH and LOW bytes in UBRR0 LOW and HIGH registers
	UBRR1L = (unsigned char)UBRR_VAL;
	UBRR1H = (unsigned char)(UBRR_VAL >> 8);
	
	// set databits
	
	// clear databits size registers before initialization
    UCSR1C &= ~((1 << UCSZ11) | (1 << UCSZ10));
    UCSR1B &= ~(1 << UCSZ12);
	
	// encoding for the 5 databits sizes supported (5, 6, 7, 8, 9)
	switch(databits) {
		case 5:
		break;	// 000 (default)
		
		case 6:
		UCSR1C |= (1 << UCSZ10);  // 001
		break;
		
		case 7:
		UCSR1C |= (1 << UCSZ11);  // 010
		break;
		
		case 8:
		UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);  // 011
		break;
		
		case 9:
		UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);  // 111
		UCSR1B |= (1 << UCSZ12); 
		break;
	}

	// set parity
    
	// clear parity registers before initialization
	UCSR1C &= ~((1 << UPM11) | (1 << UPM10));
	
	// encoding for the 3 parity modes supported (none, odd, even)
	switch(parity) {
		case 0:
		break;	// 00 (default)
		
		case 1:
		UCSR1C |= ((1 << UPM11) | (1 << UPM10));  // 11
		break;
		
		case 2:
		UCSR1C |= (1 << UPM11);  // 10
		break;
	}
	
    // set stopbits
	
	// encoding for the 2 stopbits quantities supported (1,2)
	switch(stopbits) {
		case 1:
		UCSR1C &= ~(1 << USBS1);	// 0
		break;	
		
		case 2:
		UCSR0C |= (1 << USBS0);  // 1
		break;
	}    
	
	// finally, enable receiver and transmitter
    
	UCSR1B |= ((1 << RXEN1) | (1 << TXEN1));
}

// This function waits for space in the USART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.
void uart_tx(unsigned char data) {
    /* your code here */
	
	// wait for space in buffer
	while(!(UCSR1A & (1 << UDRE1)));
	
	// load buffer with data
	UDR1 = data;	
	return;
}

// This function blocks until a character is received in the USART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.
unsigned char uart_rx(void) {
	/* your code here */
    
	// wait for space in buffer
    while (!(UCSR1A & (1 << RXC1)));
    
    // return received data from buffer
    return UDR1;
}

// This function makes it easier to print strings using uart_tx
void uart_print_string(const char* str) {
    while (*str) {
        uart_tx(*str);
        str++;
    }
}

// This function prints a startup message
void uart_print_startup(){
	
	uart_print_string("\r\n");
	uart_print_string("RS232 Communications Server\r\n");
	uart_print_string("Status: Active\r\n");
	uart_print_string("Drew Dimino\r\n");
	uart_print_string("EECE3624\r\n");
	uart_print_string("Fall 2026\r\n");
	uart_print_string("\r\n");
}