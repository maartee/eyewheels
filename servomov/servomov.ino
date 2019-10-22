/*
  
*/

#include <Servo.h>
#include <Arduino.h>


Servo servoX;  // create servo object to control a servo
Servo servoY;  // create servo object to control a servo
// int val;    // variable to read the value from the analog pin
int maxcycles = 5; //set by pc?. as variable ? -  this value limits max cycles before return to neutrál
int counter;  //used for maxcycles counting
int maxduration = 8000; // this value we get from pc
int neutralpos = 90;  //servo middle position
int neutraltime = 300;  // how long we stay at middle position after move
int xpos, ypos, duration;
int cycledelay, delaycounter;  //  for the move duration cycle


void setup() {
  // set initial values:
  xpos = ypos = neutralpos; // set to middle position
  cycledelay = 100;  // intervals, in which we check serial for new data, until 'duration' time is reached
  // initialize servos:s
  servoX.attach(8); 
  servoX.write(79);                  // sets the servo position according to the scaled value
  servoY.attach(9); 
  servoY.write(80); 
  
  delay(1500); // let the servos move to position
  // initialize serial comm:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void loop() {
  delay(1000);
}
