/*
 * USART.c
 *
 * Created: 9/20/2017 2:51:38 PM
 *  Author: madiv
 */ 
#define BAUD 9600
#define F_CPU 16000000UL

#include <util/setbaud.h>

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "GPS-USART.h"

/* Static Variables */
static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;

void USART0_Init(unsigned char ubrr)
{
	unsigned char x;

	/* Set the baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable UART receiver and transmitter */
	UCSR0B = ((1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0));
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
	//Double speed mode
	UCSR0A = (1<<U2X0);
	/* Flush receive buffer */
	x = 0;

	UART_RxTail = x;
	UART_RxHead = x;
	UART_TxTail = x;
	UART_TxHead = x;
}

/* Interrupt handlers */
ISR(USART0_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;

	/* Read the received data */
	data = UDR0;
	/* Calculate buffer index */
	tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;
	/* Store new index */
	UART_RxHead = tmphead;

	if (tmphead == UART_RxTail) {
		/* ERROR! Receive buffer overflow */
	}
	/* Store received data in buffer */
	UART_RxBuf[tmphead] = data;
}

ISR(USART0_UDRE_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if (UART_TxHead != UART_TxTail) {
		/* Calculate buffer index */
		tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
		/* Store new index */
		UART_TxTail = tmptail;
		/* Start transmission */
		UDR0 = UART_TxBuf[tmptail];
		} else {
		/* Disable UDRE interrupt */
		UCSR0B &= ~(1<<UDRIE0);
	}
}

///////////////////////////////////////////////////////////////////////////

void USART0_Transmit(unsigned char data)

{
	unsigned char tmphead;
	/* Calculate buffer index */
	tmphead = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (tmphead == UART_TxTail);
	/* Store data in buffer */
	UART_TxBuf[tmphead] = data;
	/* Store new index */
	UART_TxHead = tmphead;
	/* Enable UDRE interrupt */
	UCSR0B |= (1<<UDRIE0);
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART0_Transmit(*StringPtr);
		StringPtr++;
	}
}

///////////////////////////////////////////////////////////////////////////

int uart0_putchar(char c, FILE *stream) 
{
// 	if (c == '\n') {
// 		uart0_putchar('\r', stream);
// 	}
// 	/* Wait for empty transmit buffer */
// 	while ( !( UCSR0A & (1<<UDRE0)));
// 	/* Put data into buffer, sends the data */
// 	UDR0 = c;
	
		unsigned char tmphead;
		/* Calculate buffer index */
		tmphead = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
		/* Wait for free space in buffer */
		while (tmphead == UART_TxTail);
		/* Store data in buffer */
		UART_TxBuf[tmphead] = c;
		/* Store new index */
		UART_TxHead = tmphead;
		/* Enable UDRE interrupt */
		UCSR0B |= (1<<UDRIE0);	
}

int uart0_getchar(FILE *stream) 
{
// 	/* Wait for data to be received */
// 	while ( !(UCSR0A & (1<<RXC0)));
// 	/* Get and return received data from buffer */
// 	return UDR0;
	
		unsigned char tmptail;
		
		/* Wait for incoming data */
		while (UART_RxHead == UART_RxTail);
		/* Calculate buffer index */
		tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
		/* Store new index */
		UART_RxTail = tmptail;
		
		/* Return data */
		return UART_RxBuf[tmptail];
}

unsigned char DataInReceiveBuffer( void )
{
	return ( UART_RxHead != UART_RxTail ); //Return 0 (FALSE) if the receive buffer is empty
}
