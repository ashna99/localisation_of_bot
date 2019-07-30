#include<avr/io.h>
#include<util/delay.h>
#include<stdlib.h>
#include<string.h>
#include<avr/interrupt.h>
#include<avr/sfr_defs.h>
#include<math.h>
#define lcofangle 0
#define lcofdist 0
int countr=0,countl=0,right_channelA=0,left_channelA=0,right_channelB=0,left_channelB=0;
float x=0.0, y=0.0, theta=0.0, d_theta=0.0;
int init;
int a=0,b=0;
char snum[5];
char d;

// define some macros
#define BAUDrate 9600 
#define F_CPU 8000000UL                                 // define baud
#define ubrr 51
//char string1[10];
// function to initialize UART
void uart_init (int baud)
{   // to establish uart 0
	UBRR0H = (unsigned char)(baud>>8);                      // shift the register right by 8 bits
	UBRR0L =(unsigned char) baud;                           // set baud rate
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);                     // 4 rxen and 3 txen
	
	               // enable receiver and transmitter
	UCSR0C=0b10000110;
	
	 /* // to establish uart 1
	UBRR1H = (unsigned char)(baud>>8);                      // shift the register right by 8 bits
	UBRR1L =(unsigned char) baud;                           // set baud rate
	UCSR1B |= (1<<RXEN1)|(1 << TXEN1); 
	// enable receiver and transmitter
	UCSR1C=0b10000110;   // 8bit data format*/
}



// function to send data
void USART_Transmit( unsigned char data )

{

	/* Wait for empty transmit buffer */

	while ( !( UCSR0A & (1<<UDRE0)) );
	
	
	
	
		UDR0 = data;
		//_delay_ms(10);
	
}

	// function to receive data


unsigned char USART_Receive( void )

{

	// Wait for data to be received 

	while ( !(UCSR0A & (1<<RXC0)) );

	 ///Get and return received data from buffer 
	
	return UDR0;

}
/*void USART_Receive_String(char *string)
{	
		int i=100;
		while(i)
		{
			*string=USART_Receive();
			if(*string==i)
			break;
			string++;
			i--;
		}
}
*/
void clearstring(char *string)
{
	memset(string,0,strlen(string));
}




void usart_transmit_string(char *str)
{
	while(*str > 0)	
	{
		USART_Transmit(*str);
		str++;
		_delay_us(100);
	}
}

 

int main()
{ 
	sei(); //CALLING INTERRUPT VECTOR
	 // char str[10], 
	//char str1[100];
	//int i=0;
	uart_init(51);
	//char a;
	
	DDRD &= ~(1<<PD5); //INPUT B		PIN D5
	DDRD &= ~(1<<PD2); //INPUT B		PIN D2
	//DDRD |= (1<<PD5);		//PIN D5 FOR led 5
	
//	DDRD |= (1<<PD7);		//PIN D7 for led 7
    TCCR1B |= (1<<CS11); // pre scaling 8
	TCNT1=53035;		//initial point of count 
	TIMSK |= (1<<TOIE1);	//Timer overflow interrupt enable


	EIMSK |= (1<<INT0);  //ENABLING INTERRUPT Right wheel INPUT A	PIN D0
	EICRA |= (1<<ISC00) ; //calling interrupt at Logic change
	
	EIMSK |= (1<<INT3);  //ENABLING INTERRUPT Left wheel INPUT A	PIN D3
	EICRA |= (1<<ISC30) ; //calling interrupt at Logic change
	
	/* Replace with your application code */
	
//	init=bit_is_clear(PIND,2);

	while(1)
	{	 
		
		
	
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1=53035;
	a= countl;b=countr;
	countr=0;
	countl=0;
	d_theta = (a-b)*lcofangle;                                                   // calculating d_theta
	
	x+=(a+b)*sin(d_theta/2)*lcofdist*sin(theta + (d_theta/2))/d_theta;           // incrementing in x
	
	y+=(a+b)*sin(d_theta/2)*lcofdist*cos(theta + (d_theta/2))/d_theta;			// incrementing in y
	
	theta+=d_theta;
}
ISR(INT1_vect)
{
		right_channelB =bit_is_set(PIND,2);
		right_channelA =bit_is_set(PIND,0);
		
		if(right_channelA==right_channelB)		//clockwise
		{
			countr++;
			
			
		}
		//anticlockwise
		else if(right_channelA!=right_channelB)
		{
			countr--;
			
			
		}
		

	// convert 123 to string [buf]
	/*itoa(count, snum, 10);
	usart_transmit_string(snum);
	USART_Transmit("__");*/
	//_delay_ms(1000);
	//clearstring(sn	um);
}
ISR(INT0_vect)
{
	left_channelA=bit_is_set(PIND,3);
	left_channelB=bit_is_set(PIND,5);
	
	if(left_channelA==left_channelB)		//clockwise
	{
		countl++;
		
		
	}
	//anticlockwise
	else if(left_channelA!=left_channelB)
	{
		countl--;
		
		
	}
	

	
	/*itoa(count, snum, 10);
	usart_transmit_string(snum);
	USART_Transmit("__");
	clearstring(snum);
	*/
}