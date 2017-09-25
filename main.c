/*
 * GSM-USART.c
 *
 * Created: 9/20/2017 2:50:45 PM
 * Author : madiv
 */ 
#define F_CPU 1000000UL

#define FOSC 2000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "USART.h"

static FILE uart0_output = FDEV_SETUP_STREAM(uart0_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE uart0_input = FDEV_SETUP_STREAM(NULL, uart0_getchar, _FDEV_SETUP_READ);
char sample_GPS_data (void);

int main(void)
{
	USART0_Init(MYUBRR);
	stdout = &uart0_output;
	stdin  = &uart0_input;
		
	sei();
		
	_delay_ms(3000);
/*puts("Starting GPS!\n\r");*/
    while (1) 
    {
	    sample_GPS_data ();	
    }
}


char sample_GPS_data (void)
{
		int i=0;
		char input;
		char X;
		char BUFF[100];
	    /*		USART_putstring();*/
	    input = getchar();
	    if(input == 0x024) //if the character is "$"
	    {  /*printf("%c", input);*/	input = getchar();
		    if(input == 0x047) //if the character is "G"
		    {	/*printf("%c", input);*/	input = getchar();
			  if(input == 0x050) //if the character is "P"
			   {	/*printf("%c", input);*/	input = getchar();
				 if(input == 0x047) //if the character is "G"
				  {	/*printf("%c", input);*/	input = getchar();
					if(input == 0x047) //if the character is "G"
					 {	/*printf("\r\nGPGGA: ");*/	input = getchar();
						while (input != 0x024)
						 {	input = getchar(); //printf("%c", input);
							X = input;
							BUFF[i] = X;
							i++;
						 }
						printf("\r\nBUFFER START\r\n");
 						printf(BUFF);
 						printf("\r\nBUFFER END\r\n");
					 }
					else{}}
				else{}}
			else{}}
		 else{}}
	 else{}
return 0;
}

