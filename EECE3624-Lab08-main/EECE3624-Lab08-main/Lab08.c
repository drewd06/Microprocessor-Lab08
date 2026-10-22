/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified:
 *   ModDate:
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 9600 // standard values from 1200 to 115200
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

#define ESC 27	// ASCII 27 = ESC key

/* Your code here.  */

int main(void)
{
	/* Your code here.  */
	
	// configure LED0 as output
	DDRC |= (1 << PC0);
	// initialize OFF
	PORTC &= ~(1 << PC0);
	
	// store the last received character
	unsigned char received;
	// track next expected "end" character
	unsigned char end_count = 0;
	
    /* See the UARTLibrary.h for available calls.
	 *     	  // initialize the UART
	 *     	  uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	 *
	 *        // transmit a value (0x41) to the comm port
	 *        c = 0x41;
	 *        uart_tx(c);
	 *
	 *        // receive a byte from the comm port
	 *        c = uart_rx();
	 */

	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	
	// startup message
	uart_print_startup();
	
	while(1)
	{
		/* your loop code here */
		
		// receive next character
		received = uart_rx();
		
		// blink LED0 to indicate received character
		PORTC |= (1 << PC0);	// ON
		_delay_ms(75);			// wait 75ms
		PORTC &= ~(1 << PC0);	// OFF
		
		// ESC check: Restart server
		if (received == ESC){
			uart_print_startup();
			end_count = 0;
		}
		
		// "end" check: Terminate server
		// end_count = 0: look for 'e'
		if (received == 'e' && end_count == 0) {
			end_count = 1;
		} 
		// end_count = 1: look for 'n'
		else if (received == 'n' && end_count == 1) {
			end_count = 2;
		} 
		// end_count = 2: look for 'd'
		else if (received == 'd' && end_count == 2) {
			
			// Complete 'end' sequence detected
			uart_print_string("\r\nBye\r\n");
			while(1);  // Halt (infinite loop)
			
		}	
		// not an "end" character: reset count	
		else {
			end_count = 0; 
		}
		// echo received character
		uart_tx(received);
	}
	return 0;
}
