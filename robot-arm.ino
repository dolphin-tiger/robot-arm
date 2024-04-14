/****************************************************************************************************
  @file robot-arm.ino
  @brief main code for controlling the servo based robot arm
  @author Jeremy Reynolds <62484970+jeremy-reynolds@users.noreply.github.com>
  @version 1.0.7
  @date 2024/04/14

  @details
  robot-arm is a 5th grade GT class project to build and control a robotic arm utilizing an Arduino controller, 
  servo motors, and analog joysticks. The original goal of the project was to learn how coding works, with the 
  specific request to start from scratch as opposed to modifying something existing.  We mostly accomplished that
  task here by creating our own includes but also learned why at some point it is impractical and undesirable to 
  recreate all the code from scratch. This project has a good mixture of scratch code along with libraries like 
  from Adafruit and other even more obvious like <Arduino.h>.

  # LICENSE #
  
  MIT License

  Copyright (c) 2024 dolphin-tiger

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

****************************************************************************************************/

#include <Arduino.h>
#include "joystick.h"
#include "robotMotor.h"
#include <Adafruit_NeoPixel.h>

/*----------------------------------------------------------------------------------------------------
--- define the joystick objects
------------------------------------------------------------------------------------------------------*/
joystick joy1(A0, A1, 2);
joystick joy2(A2, A3, 3);

/*----------------------------------------------------------------------------------------------------
--- define the motor objects
------------------------------------------------------------------------------------------------------*/
typedef enum motorAxis:uint8_t {X1=0, Y1, Y2} motorAxis_t;  //-- enum to use for motor labels
robotMotor motor[3];
int pwm_i2c = 0x40;

/*----------------------------------------------------------------------------------------------------
--- variables for timer to use millis for non-blocking code
------------------------------------------------------------------------------------------------------*/
long joyReadTime = 0;
long joyReadDelay = 50;
long btnReadTime = 0;
long btnReadDelay = 50;

/*----------------------------------------------------------------------------------------------------
--- variables for what mode the motors are in.
----- Level Mode
---   - true: second Y motor will move to 90deg from first Y motor
---   - false: second Y motor will be controlled by Joystick 2
------------------------------------------------------------------------------------------------------*/
bool motorDisable    = true;
int motorDisablePin  = 24;
bool levelMode       = false;
int levelMode_offset = 55;

/*----------------------------------------------------------------------------------------------------
--- neopixel objects
------------------------------------------------------------------------------------------------------*/
Adafruit_NeoPixel neo(8, 22,NEO_GRBW + NEO_KHZ800);
int32_t levelMode_color     = neo.Color(  0,255,  0);
int32_t normalMode_color    = neo.Color( 80,  0,255);
int32_t motorDisable_color  = neo.Color(255,  0,  0);
/*----------------------------------------------------------------------------------------------------
--- setup code to run once 
------------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  
  //-- setup joysticks -------------------------------------------------------------------------------
    Serial.println("Calibrating Joysticks...");
      //joy1.debug = true;
      joy1.calibrateCenter();
      joy2.calibrateCenter();

  //-- setup servos after calibration ----------------------------------------------------------------
    Serial.println("Attaching Servos...");

    //-- setup x-motor -------------------------------
      motor[X1].setMinPosition(0);
      motor[X1].setMaxPosition(180);
      motor[X1].setCenterPosition(90);
      motor[X1].attach(0x40, X1);

    //-- setup y1-motor -------------------------------
      motor[Y1].setMinPosition(90);
      motor[Y1].setMaxPosition(170);
      motor[Y1].setCenterPosition(110);
      motor[Y1].attach(0x40, Y1);

    //-- setup y2-motor -------------------------------
      motor[Y2].setMinPosition(0);
      motor[Y2].setMaxPosition(180);
      motor[Y2].setCenterPosition(85);
      motor[Y2].attach(0x40, Y2);
      motor[Y2].setPosition( map( motor[Y1].getPosition(), 0, 180, 180, 0) + levelMode_offset );

    //-- setup other things in the code -----------------
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN,levelMode);
      neo.begin();

      delay (10);
      pinMode(motorDisablePin, OUTPUT);

      delay (10);
      digitalWrite(motorDisablePin, motorDisable);

      delay (10);
      ledColor();
}

/*----------------------------------------------------------------------------------------------------
--- main code to run in a loop 
------------------------------------------------------------------------------------------------------*/
void loop() {
  //--- using millis() to read joystick every 1/20th of a second and avoid blocking in the code
  if( millis() - joyReadTime >= joyReadDelay && motorDisable == false) {
    joyReadTime = millis();

    //--- read the joystick and scale the output to the specified range (range is optional)
      int16_t x1 = joy1.getPosition(X, -10, 10, true);
      int16_t y1 = joy1.getPosition(Y, -10, 10);

      int16_t x2 = joy2.getPosition(X, -10, 10, true);
      int16_t y2 = joy2.getPosition(Y, -10, 10);

    //-- motor[Xn] movements
      if(x1 != 0) {
        motor[X1].moveInc(x1);
      }
      if(x2 != 0 && levelMode == true) {
        motor[X1].moveInc(x2);
      }
      
    //-- motor[Yn] movements and levelMode
      if(levelMode == true){
        if(y1 != 0) {
          motor[Y1].moveInc(y1);
          motor[Y2].setPosition( map( motor[Y1].getPosition(), 0, 180, 180, 0) + levelMode_offset );
        }
        if(y2 != 0) {
          motor[Y1].moveInc(y2);
          motor[Y2].setPosition( map( motor[Y1].getPosition(), 0, 180, 180, 0) + levelMode_offset );
        }
      }
      else {
        if (y1 != 0) motor[Y1].moveInc(y1);
        if (y2 != 0) motor[Y2].moveInc(y2);
      }
    

    //--- print the output if it is not at the neutral position
    
      if(x1 != 0 || y1 != 0) { Serial.print("Joy1 x1:"); Serial.print(x1); Serial.print(", y1:"); Serial.print(y1); Serial.print(", time:"); Serial.println(millis());}
      if(x2 != 0 || y2 != 0) { Serial.print("Joy2 x2:"); Serial.print(x2); Serial.print(", y2:"); Serial.print(y2); Serial.print(", time:"); Serial.println(millis());}
    
  }

  if( millis() - btnReadTime >= btnReadDelay ) {
    btnReadTime = millis();
    //--- check for button presses
      int16_t b1 = joy1.getButton();
      int16_t b2 = joy2.getButton();

    //--- enable or disable motors
      if(b1 == true) {
        motorDisable = !motorDisable;
        digitalWrite(motorDisablePin, motorDisable);
      }

    //--- enable or disable levelMode
      if(b2 == true && motorDisable == false) {

        //-- change levelMode to opposite value
        levelMode = !levelMode;

        //-- set position of motor[Y2] to 90deg from motor[Y1]
        motor[Y2].setPosition( map( motor[Y1].getPosition(), 0, 180, 180, 0) + levelMode_offset );

        //-- use the built-in LED on the board to display the levelMode state
        digitalWrite(LED_BUILTIN, levelMode);
      }

      ledColor();

      if(b1) Serial.println("Button 1 Pressed!");
      if(b2) Serial.println("Button 2 Pressed!");
  }

}

/*----------------------------------------------------------------------------------------------------
--- function to determine and se the color of the LED's
------------------------------------------------------------------------------------------------------*/
void ledColor(){
  if(motorDisable == true) {
    neo.fill(motorDisable_color); 
  }
  else if (levelMode == true) {
    neo.fill(levelMode_color); 
  }
  else if (levelMode == false) {
    neo.fill(normalMode_color); 
  }
  neo.show();
}