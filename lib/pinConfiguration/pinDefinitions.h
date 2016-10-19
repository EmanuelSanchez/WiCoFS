//Pins Definitions Arduino UNO, Arduino Nano/*

// ARDUINO UNO

// Digital Ports:
/*
TX1 - RX0 : DDRD
D2 - D7 : DDRD
D8 - D13 : DDRB
RST : DDRC
A0 - A5 : DDRC
 */

/********** Digital Pins **********/

// DDD
#define RX0 0   //Mode Serial Comunication
#define TX1 1

#define D0  0   //Mode I/O pots
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7

#define _PCINT16 0
#define _PCINT17 1
#define _PCINT18 2
#define _PCINT19 3
#define _PCINT20 4
#define _PCINT21 5
#define _PCINT22 6
#define _PCINT23 7

// DDB
#define D8  0   //Mode I/O pots
#define D9  1
#define D10 2
#define D11 3
#define D12 4
#define D13 5

#define _PCINT0 0  //Mode Digital Interrupt
#define _PCINT1 1
#define _PCINT2 2
#define _PCINT3 3
#define _PCINT4 4
#define _PCINT5 5

/********** Analog Pins **********/

// DDC
#define _A0 	0
#define _A1 	1
#define _A2 	2
#define _A3 	3
#define _A4 	4
#define _A5	  5
#define _A6   6

#define _PCINT8  0  //Mode Digital Interrupt
#define _PCINT9  1
#define _PCINT10 2
#define _PCINT11 3
#define _PCINT12 4
#define _PCINT13 5
#define _PCINT14 6
