#include<avr/sfr_defs.h>e
//#include<avr/iom328p.h>
#include<pinConfiguration.h>
#include<serial.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<Arduino.h>

int eje_x;
int eje_y;
int eje_z;

void setup()
{
  SerialInit();
  //Serial.begin(57600);
  /*
	DigitalB_OUTPUT(D13);
  DigitalD_OUTPUT(D6);
  DigitalD_HIGH(D6);
  DigitalD_INPUT(D7);
  */
  send_string("Beginning...");
  _delay_ms(2000);
}

void loop()
{

  Analog_INPUT0();
  eje_x = Analog_read();
  send_string("Eke X:");
  send_number(eje_x);
  send_string("\n");
  Analog_INPUT1();
  eje_y = Analog_read();
  send_string("Eje Y: ");
  send_number(eje_y);
  send_string("\n");
  Analog_INPUT2();
  eje_z = Analog_read();
  send_string("Eje Z: ");
  send_number(eje_z);
  send_string("\n");
  _delay_ms(1000);

  /*
  if(DigitalD_read(D7)){
    DigitalB_HIGH(D13);
  }
  else{
    DigitalB_LOW(D13);

  }
  */

}
