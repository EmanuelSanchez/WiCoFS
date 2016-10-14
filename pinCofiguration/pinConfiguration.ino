/*

	Pin configurations (input, output)

*/

#include "pinsDefinitions.h"

#define _OUTPUT(pin) DDRB |= _BV(pin);
#define _INPUT(pin)	DDRB &= ~_BV(pin);
#define _HIGH(pin) PORTB |= _BV(pin);
#define _LOW(pin) PORTB &= ~_BV(pin);
/*
void _PULL_UP(uint8_t pin)
{
	PORTB &= ~_BV(pin);
}
*/

//////////////////////////////////////////////////////////////////////////////////////
//
//	Example of implementation
//
//////////////////////////////////////////////////////////////////////////////////////

void setup()
{	
	_OUTPUT(D13);
}

void loop()
{
	_HIGH(D13);
	_delay_ms(500);
	_LOW(D13);
	_delay_ms(500);
}