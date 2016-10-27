#include<avr/sfr_defs.h>
#include<pinConfiguration.h>
#include<serial.h>
#include<avr/interrupt.h>
#include<Arduino.h>

#include <math.h>

// smoothing factor of the Low Pass Filter
const float alpha = 0.7;
// constant defines
const float pi = 3.14;
// pins asignations
const int xpin = A0;
const int ypin = A1;
const int zpin = A2;
// offset values
const float xZero=325;
const float yZero=397;
const float zZero=410;
// acceleration varables
float xAcc;
float yAcc;
float zAcc;
// filtered accelerations
float xAcc_f = 0.0;
float yAcc_f = 0.0;
float zAcc_f = 0.0;
// angle variables
float roll;   // angle in the X-Z plane (-90, 90)
float pitch;  // angle in the X-Y plane (-180, 180)

void read_accelerometer(){
  // read the data from de accelerometer
  Analog_INPUT0();
  xAcc=((Analog_read())-xZero)/(9.8);
  Analog_INPUT1();
  yAcc=((Analog_read())-yZero)/(9.8);
  Analog_INPUT2();
  zAcc=((Analog_read())-zZero)/(9.9);
}

void lpf(){
  xAcc_f = alpha*xAcc+(xAcc_f*(1.0-alpha));
  yAcc_f = alpha*yAcc+(yAcc_f*(1.0-alpha));
  zAcc_f = alpha*zAcc+(zAcc_f*(1.0-alpha));
}

void pitch_calculate(){
  pitch=atan(xAcc_f/sqrt(pow(yAcc_f,2)+pow(zAcc_f,2)));
  pitch=pitch*(180.0/pi);
}

void roll_calculate(){
  roll=atan((yAcc_f)/sqrt(pow(xAcc_f,2)+pow(zAcc_f,2)));
  roll=roll*(180.0/pi);
}

void setup()
{
  SerialInit();
  send_stringln("Begining...");
  _delay_ms(500);
}

void loop()
{
  //read the sensor
  read_accelerometer();
  //low pass filter
  lpf();
  pitch_calculate();
  send_numberln(pitch);
}
