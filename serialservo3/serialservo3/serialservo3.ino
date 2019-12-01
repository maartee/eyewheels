/*
  Sketch drives two servos , that are set to move joystick in x and y direction. 
  // Accepts message from serial port , in following frm (xposition:yposition:duration)  After,, it reerd
*/

#include <Servo.h>
#include <Arduino.h>


Servo servoX;  // create servo object to control a servo - Forward, back
Servo servoY;  // create servo object to control a servo - left  , right

int maxduration = 8000; // the maximum move duration
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
  servoX.write(neutralpos);                  // sets the servo position according to the scaled value
  servoY.attach(9); 
  servoY.write(neutralpos); 
  
  delay(1500); // let the servos move to position
  // initialize serial comm:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void loop() {
 
      // Read serial input - three integer values between ( ) otherwise skip to next: 
  if (Serial.available() > 0) {
    char inChar = Serial.read(); // read one char
    Serial.print("inChar: ");
    Serial.println(inChar);
    // check if we are at start of the message:
    if (inChar == '(') {
      // parsing message (xpos:ypos:duration) 
      xpos = Serial.parseInt(); // read 1st integer
      ypos = Serial.parseInt(); // read 2nd integer
      duration = Serial.parseInt();  // read 3rd integer
      inChar = Serial.read();  // read end of message
      // end of the message:
      if (inChar == ')') {
        // print for debug:
        Serial.print("xpos: ");
        Serial.println(xpos); 
        Serial.print("ypos: ");
        Serial.println(ypos);
        Serial.print("duration: ");
        Serial.println(duration);

        // we need to check  values, then move servos:
        if ((xpos <= 8) and (xpos >= 0) and (ypos <= 8) and (ypos >= 0) 
               and (duration <= maxduration) and (duration >= 0)) {
          xpos = map(xpos, 0, 8, 60, 120);     // scale it to use it with the servo (value between 0 and 180)
          servoX.write(xpos);   
          ypos = map(ypos, 0, 8, 50, 130);     // scale it to use it with the servo (value between 0 and 180)
          servoY.write(ypos); // sets the servo position according to the scaled value
         
          
          // clear any other chars from serial:
          // debug
          inChar = Serial.peek();
          Serial.print("peek: ");
          Serial.println(inChar);
         
          while ((inChar != -1) and (inChar != '(')) {
            inChar = Serial.read();
            Serial.print("read: ");
            Serial.println(inChar);
          }

          // Now we wait - move duration. It's broken into short 'cycledelay' delays, 
          // cause we want to be able to stop or change direction quickly any time,
          // we can use switch or serial etc.:
          delaycounter = 0;
          while ((inChar = Serial.peek() == -1) and ((cycledelay*delaycounter) <= duration)) {
            delay(cycledelay);
            ++delaycounter;
            Serial.print("peek2: ");
            Serial.println(inChar);
          }
          servoX.write(neutralpos); 
          servoY.write(neutralpos);// return to neutral
          // delay(neutraltime); //without this, the servo has not time to return,
          // if there's another serial input - this maybe desirable for smooth ride
        }     
      }
    }
  }
} 
