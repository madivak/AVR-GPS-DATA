/*
 * GPS-UART-BUF.c
 *
 * Created: 9/23/2017 5:37:46 PM
 * Author : madiv
 */ 

#define F_CPU 1000000UL

#define FOSC 2000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

#include <avr/io.h>
#include <stdio.h>
/*#include <string.h>*/
#include <util/delay.h>
#include <avr/interrupt.h>

#include "GPS-USART.h"
char input;

static FILE uart0_output = FDEV_SETUP_STREAM(uart0_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE uart0_input = FDEV_SETUP_STREAM(NULL, uart0_getchar, _FDEV_SETUP_READ);

/*char BUFF[100];*/

char sample_GPS_data (void);


int main(void)
{
	    USART0_Init(MYUBRR);
	    stdout = &uart0_output;
	    stdin  = &uart0_input;
		
		sei();
		
		_delay_ms(3000);
		while (1)
		{	
			sample_GPS_data ();
			_delay_ms(4000);				
		}
}

char sample_GPS_data (void)
{
			int i=0;
			printf("AT\r\n");
			_delay_ms(2000);
			printf("AT+CGATT=1\r\n");
			_delay_ms(2000);
			printf("AT+CIPMUX=0\r\n");
			_delay_ms(2000);
			printf("AT+CSTT=\"APN\",\"\",\"\"\r\n");
			_delay_ms(2000);
			printf("AT+CIICR\r\n");
			_delay_ms(2000);
			printf("AT+CIFSR\r\n");
			_delay_ms(2000);
			printf("AT+CIPSTART=\"TCP\",\"IP\",\"PORT\"\r\n");
			_delay_ms(2000);
			printf("AT+CIPSEND\r\n");
			_delay_ms(2000);
			printf("\r\n$GPGG");
			while(i == 0)
			{
				input = getchar();
				if (input == 0x024) //if the character is "$"
				{ 
					input = getchar();
					if (input == 0x047) //if the character is "G"
					{
						input = getchar();
						if (input == 0x050) //if the character is "P"
						{
							input = getchar();
							if (input != 0x047) //if the character is not "G"
							{}
							else
							{
								input = getchar();
								if (input != 0x047) //if the character is not "G"
								{} 
								else
								{
									input = getchar();
									while (input != 0x024)  //if the character is not "$"
									{
										putchar(input);
										input = getchar();
										i=1;
									}
								}
							}							
						}
						else{}
					}
					else{}
				}
				else{}
			}
		putchar(0x1A); //putting AT-MSG termination CTRL+Z in USART0
		_delay_ms(2000);
		printf("AT+CIPCLOSE\r\n");
		_delay_ms(2000);
return 0;
}
