#include<avr/sfr_defs.h>
#include<pinConfiguration.h>
#include<serial.h>
#include<avr/interrupt.h>
#include<Arduino.h>

#include <math.h>

#define sensibility 2.5*9.8
#define Nsamples 10
#define timeout 15000// time limit to ultrasound sensor (microseconds)


// pins asignations
#define xpin A0
#define ypin A1
#define zpin A2
#define ultrasoundEcho D11
#define ultrasoundTrig D12

// smoothing factor of the Low Pass Filter
const float alpha = 0.7;
// constant defines
const float pi = 3.14;

// accelerometer offset values
// this values can change for different reasons
// (one can be the cahnges in power) for this we
// recommend calibrate each time that the environment or power conditions change

// values to USB power
// #define xZero_default 325
// #define yZero_default 397
// #define zZero_default 410
// values to USB power 2
// #define xZero_default 104  111  323
// #define yZero_default 431  432  430
// #define zZero_default 437  440  430
//values to battery power
// #define xZero_default 325
// #define yZero_default 397
// #define zZero_default 485
// #define xZero_default 320
// #define yZero_default 397
// #define zZero_default 408

//values to battery power
float xZero = 320;
float yZero = 397;
float zZero = 408;

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
// ultrasound lecture
int ultrasoundValue;
int ultrasoundZero;

// #define ultrasoundZero_dafult
// variable to save the message from serial port
char cmd[5];

void waitMillis(uint32_t time_milis){
  uint32_t initialTime;
  uint32_t timer = 0;
  initialTime = millis();
  while(timer<=time_milis){
    timer = millis() - initialTime;
  }
}

void waitMicros(uint32_t time_micros){
  uint32_t initialTime;
  uint32_t timer;
  initialTime = micros();
  while(timer<=time_micros){
    timer = micros() - initialTime;
  }
}

long pulsewidth(int pin, bool state){
  unsigned long duration;
   if(state == HIGH){                                        //                           __
     while(!DigitalB_read(pin)){}      // wait for raising edge  __|
     duration = 0;
     while(DigitalB_read(pin)){       // wait for falling edge  __|    |__
      if(duration>timeout)
        return 0;
      duration += 1;
     }
   }
   else{                                              //                        __
     while(DigitalB_read(pin)){}      // wait for falling edge    |__
     duration = 0;
     while(!DigitalB_read(pin)){     // wait for falling edge    |____|
        if(duration>timeout)
          return 0;
        duration += 1;

     }
   }
   return duration;
}

void read_accelerometer(){
  // read the data from de accelerometer
  Analog_INPUT0();
  // xAcc=Analog_read();
  xAcc=((Analog_read())-xZero)/(sensibility);
  Analog_INPUT1();
  // yAcc=Analog_read();
  yAcc=((Analog_read())-yZero)/(sensibility);
  Analog_INPUT2();
  // zAcc=Analog_read();
  zAcc=((Analog_read())-zZero)/(sensibility);
}

void read_ultrasound(){
  unsigned long durationEcho = 0;
  DigitalB_LOW(ultrasoundTrig);
  waitMillis(1);
  DigitalB_HIGH(ultrasoundTrig);
  waitMillis(10);
  DigitalB_LOW(ultrasoundTrig);
  durationEcho = pulsewidth(ultrasoundEcho, HIGH);
  ultrasoundValue = durationEcho/29.1/2;
}

void waitInit(){
  while(string_receiver()!='b'){
  }
  send_stringln("y");
}

// void waitRequest(){
//   cmd[0]=string_receiver();
//   while(cmd[0]!='r'){
//     cmd[0]=string_receiver();
//     // send_string(cmd);
//   }
//   send_stringln("o");
// }

void waitRequest(){
  while(string_receiver()!='r'){
  }
  send_stringln("o");
}

void response(){
  while(string_receiver()!='w'){
  }
  send_stringln("e");
}

void send_data(){
  send_stringln("s");
  send_number(roll);
  send_string(",");
  send_number(pitch);
  send_string(",");
  send_numberln(ultrasoundValue-ultrasoundZero);
}

void send_data1(){
  send_stringln("s");
  send_number(xAcc_f);
  send_string(",");
  send_number(yAcc_f);
  send_string(",");
  send_numberln(zAcc_f);
}

void send_4data(int value1, int value2, int value3, int value4){
  waitRequest();
  send_number(value1);
  send_string(",");
  send_number(value2);
  send_string(",");
  send_number(value3);
  send_string(",");
  send_numberln(value4);
  response();
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

// function to calibrate the zero values of the X and Y axis (Z isn't calibrate
// because to change the accelerometer position to it)
void calibration_routine(){
  float readingx = 0;
  float readingy = 0;
  float readingz = 0;
  float readingultrasound = 0;

  waitRequest();
  waitMillis(5000);

  for(int i=0; i<Nsamples; i++){
    Analog_INPUT0();
    readingx += Analog_read();
    Analog_INPUT1();
    readingy += Analog_read();
    read_ultrasound();
    readingultrasound += ultrasoundValue;
  }

  xZero = readingx/Nsamples;
  yZero = readingy/Nsamples;
  ultrasoundZero = readingultrasound/Nsamples;

  response();

  waitRequest();    //send string to avise right calibration x and y axis
  waitMillis(5000);

  for(int i=0; i<Nsamples; i++){
    Analog_INPUT2();
    readingz += Analog_read();
  }

  zZero = readingz/Nsamples;

  response();

  send_4data(xZero, yZero, zZero, ultrasoundZero);
}

void blink(int N){
  for(int i=0; i<N;i++){
    DigitalB_HIGH(D13);
    waitMillis(35);
    DigitalB_LOW(D13);
    waitMillis(35);
  }
}

void setup()
{
  blink(5);
  SerialInit();
  DigitalB_INPUT(ultrasoundEcho);
  DigitalB_OUTPUT(ultrasoundTrig);
  DigitalB_OUTPUT(D13);
  waitInit();      // wait signal to start
  while(1){
    cmd[0]=string_receiver();
    if(cmd[0]=='c'){
      send_stringln("c");
      blink(2);
      calibration_routine();
      break;
    }
    else if(cmd[0]=='n'){
      send_stringln("n");
      blink(3);
      break;
    }
  }
}

void loop()
{
  //read the sensor
  read_accelerometer();
  read_ultrasound();
  //low pass filter
  lpf();
  pitch_calculate();
  roll_calculate();
  send_data();
  // send_stringln("Datos:");
  // send_number(xACC_f);
  // send_string(",");
  // send_number(pitch);
  // send_string(",");
  // send_numberln(ultrasoundValue);
}
