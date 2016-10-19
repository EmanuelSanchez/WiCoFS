/*

    UART Serial Comunication in C

Note: in the function send_number never send a number that starts with zero,
because it is understand like base octal.

*/

#define BAUD 9600     // Define the Comunication Baud Rate
#define BVV(bit, val) ((val)?_BV(bit):0)  // function auxiliar


#include <util/setbaud.h>
#include <stdio.h>
#include <arduino.h>


void init(void )  // Initialize the serial comunication
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~_BV(U2X0);
#endif
   UCSR0B = BVV(TXEN0, 0) | BVV(RXEN0, 1); // Enable Rx and disable Tx
   //UCSR0C = BVV(USBS0, 1) | BVV(UCSZ00, 3); // Set frame format: 8 data, 2 stop bit
}

void string_receiver(const char *s)   // Receive a character or a string
{
    while(*s != '\0')
    {
        while(!(UCSR0A & _BV(RXC0))); // Wait for data to be received
            return UDR0;// Get and return received data from buffer
    }
}


//////////////////////////////////////////////////////////////////////////////////////
//
//  Example of implementation
//
//////////////////////////////////////////////////////////////////////////////////////

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
