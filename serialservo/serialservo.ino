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


void setup() {
  // set initial values:
  xpos = ypos = neutralpos;
  duration = 100;
  // initialize servos:s
  servoX.attach(9); 
  servoX.write(neutralpos);                  // sets the servo position according to the scaled value
  servoY.attach(10); 
  servoY.write(neutralpos); 
  
  delay(1500); // let the servos move to position
  // initialize serial comm:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void loop() {
  //security counter- if we receive no data for maxtime, we reset servo position to 90 deg - needs revision, may be useless:
  counter = maxcycles;
      // Read serial input - three integer values between < , >, otherwise skip to next: 
  while (Serial.available() > 0) {
    Serial.print("counter: ");
    Serial.println(counter);
    --counter;
    char inChar = Serial.read();

    Serial.print("inChar: ");
    Serial.println(inChar);
    // check if we are at start of the message:
    if (inChar == '(') {
      // parsing message <xpos:ypos:duration> :
    }
      xpos = Serial.parseInt(); 
      ypos = Serial.parseInt(); 
      duration = Serial.parseInt();  
      inChar = Serial.read();
      // end of the message:
      if (inChar == ')') {
        // print for debug:
        Serial.print("xpos: ");
        Serial.println(xpos); 
        Serial.print("ypos: ");
        Serial.println(ypos);
        Serial.print("duration: ");
        Serial.println(duration);
        // we need to check  values:
        if ((xpos <= 8) and (xpos >= 0) and (ypos <= 8) and (ypos >= 0) 
        and (duration <= maxduration) and (duration >= 0)) {
          xpos = map(xpos, 0, 8, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
          servoX.write(xpos);   
          ypos = map(ypos, 0, 8, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
          servoY.write(ypos); // sets the servo position according to the scaled value
          // todo - convert the delay into for cycle, so we can check for emergency input - we can use switch or serial etc.
          delay(duration); // during this, we should check, if there's new data from serial
          // - how long is servo in Move position
          
          servoX.write(neutralpos); 
          servoY.write(neutralpos);// return to neutral - we probably need smooth ride, so 
          // Only should return if no new serial data are present,,
          // we can make this conditional using peek, e.g.
          // delay(neutraltime); //without this, the servo has not time to return,
          // if there's another serial input - this maybe desirable for smooth ride
        }
      
     }
     // does this work at all ? :
    if (counter <= 0) {
      servoX.write(neutralpos);
      servoY.write(neutralpos);
      counter = maxcycles;
      delay(neutraltime);
    }
  }
}
