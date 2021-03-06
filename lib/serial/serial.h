/*

    UART Serial Comunication in C

Note: in the function send_number never send a number that starts with zero,
because it is understand like base octal.

*/

#define BAUD 9600     // Define the Comunication Baud Rate
#define BVV(bit, val) ((val)?_BV(bit):0)  // function auxiliar

#include<avr/sfr_defs.h>
#include<util/setbaud.h>
#include<stdlib.h>
#include<stdio.h>
#include<util/delay.h>

void SerialInit(void )  // Initialize the serial comunication
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~_BV(U2X0);
#endif
   UCSR0B = BVV(TXEN0, 1) | BVV(RXEN0, 1); // Enable Tx and disable Rx
   //UCSR0C = BVV(USBS0, 1) | BVV(UCSZ00, 3); // Set frame format: 8 data, 2 stop bit
   UCSR0C = BVV(UMSEL01,0) | BVV(UMSEL00,0) | BVV(UCSZ00,1) | BVV(UCSZ01,1);
}

char string_receiver(void)   // Receive a character or a string
{
        while(!(UCSR0A & _BV(RXC0))){ // Wait for data to be received
        }
        return UDR0;// Get and return received data from buffer

}

void send_string(const char *s)   // Send a character o array of characters (string)
{
    while(*s != '\0')
    {
        while(!(UCSR0A & _BV(UDRE0)));
            UDR0 = *s++;
    }
}

void send_stringln(const char *s)   // Send a character o array of characters (string)
{
    send_string(s);
    send_string("\n");
}

void send_number(const int value)
{
  char buffer[100];         //the ASCII of the integer will be stored in this char array

  itoa(value,buffer,10); // (integer, yourBuffer, base)
  send_string(buffer);  // itoa funciont convert an int to string
}

void send_numberln(const int value)
{
  char buffer[100];         //the ASCII of the integer will be stored in this char array

  itoa(value,buffer,10); // (integer, yourBuffer, base)
  send_stringln(buffer);  // itoa funciont convert an int to string
}

//////////////////////////////////////////////////////////////////////////////////////
//
//  Example of implementation
//
//////////////////////////////////////////////////////////////////////////////////////
/*
void setup()
{
  init();
  send_string("Start Comunication:\n");
  send_string("Emanuel Sánchez");
  send_string("\n");
  send_number(1234567890);
  send_string("\n");
}

void loop()
{

}
*/
