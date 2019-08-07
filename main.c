#include<avr/io.h>
#include<util/delay.h>
#include<stdlib.h>
#include "USART_128.h"
#include<string.h>
#include<avr/interrupt.h>
#include<avr/sfr_defs.h>

volatile int count=0,pstate,laststate,stateb;
volatile int flag = 1;
int init;



int main(void)
{   USART_Init(51,0);
	DDRD &= ~(1<<PD5); //INPUT B		PIN D2
	DDRD &= ~(1<<PD4);
	laststate=bit_is_set(PIND,4);
	
    while (1) 
    {
		
		pstate=bit_is_set(PIND,4);
		  //stateb=bit_is_set(PIND,5);
			if(laststate!=pstate)		//clockwise
			{
				if(!pstate)
				{	
					if(pstate!=bit_is_set(PIND,5))
						count++;
					else 
						count--;
					USART_TransmitNumber(count,0);
					USART_Transmitchar(' ',0);
				}
				laststate=pstate;
			}
		
		
		
		
    }
}

