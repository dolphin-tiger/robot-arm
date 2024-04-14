/****************************************************************************************************
  @file robotMotor.h
  @brief Servo Motor control class utilizing pwm module
  @author Jeremy Reynolds <62484970+jeremy-reynolds@users.noreply.github.com>
  @version 1.0.0
  @date 2024/03/30

  @details
  robotMotor is a simple class written in Arduino IDE to manage servo motors for a simple robotic arm 
  application. The code utilizes a PCA9685 16ch PWM module with an I2C connection using the library 
  from Adafruit called PWMServoDriver.h.  This library allows for non blocking movements of the Servo
  motors for smoother control.

  version 1.0.0 - initial version
  
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
#include "robotMotor.h"
#include <Arduino.h>

#define MIN_PULSE_WIDTH       480   //650
#define MAX_PULSE_WIDTH       2400  //2350
#define DEFAULT_PULSE_WIDTH   1465
#define FREQUENCY             50

  robotMotor::robotMotor() {

  }

  int pulseWidth(int angle)
  {
    int pulse_wide, analog_value;
    pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
    return analog_value;
  }

  void robotMotor::attach(int i2c, int ch){
    i2cAddress = i2c;
    motorID = ch;

    position = centerPosition;

    svo = Adafruit_PWMServoDriver(i2cAddress); //redefine the object if this works?
    svo.begin();
    svo.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
    svo.setPWM(motorID, 0, pulseWidth(position));
  }


  //-- move methods ---------------------------------------------------------------------------
  void robotMotor::moveInc(int val) {
    int tempPosition = position + val;
    if      ( tempPosition > maxPosition) { position = maxPosition;}
    else if ( tempPosition < minPosition) { position = minPosition;}
    else                                  { position = tempPosition;}
    svo.setPWM(motorID, 0, pulseWidth(position));  
  }

  void robotMotor::setPosition(int pos) {
    int tempPosition = pos;
    if      (tempPosition > maxPosition)  { position = maxPosition; }
    else if (tempPosition < minPosition)  { position = minPosition; }
    else                                  { position = tempPosition;}
    svo.setPWM(motorID, 0, pulseWidth(position));
  }
  
  int robotMotor::getPosition() {
    if      (position > maxPosition)      { position = maxPosition; }
    else if (position < minPosition)      { position = minPosition; }
    else                                  { position = position;    }
    return position;
  }

  //-- center postion methods -----------------------------------------------------------------
  void robotMotor::setCenterPosition(int pos){
    int tempPosition = pos;
    if      (pos > maxPosition)           { centerPosition = maxPosition; }
    else if (pos < minPosition)           { centerPosition = minPosition; }
    else                                  { centerPosition = pos;}
  }
  int robotMotor::getCenterPosition() {
    return centerPosition;
  }

  //-- minimum position methods ---------------------------------------------------------------
  void robotMotor::setMinPosition(int pos){
    int tempPosition = pos;
    if      (pos > maxPosition)           { }
    else                                  { minPosition = pos; }
  }

  int robotMotor::getMinPosition() {
    return minPosition;
  }

  //-- maximum position methods ---------------------------------------------------------------
  void robotMotor::setMaxPosition(int pos){
    int tempPosition = pos;
    if      (pos < minPosition)           { }
    else                                  { maxPosition = pos; }
  }

  int robotMotor::getMaxPosition() {
    return maxPosition;
  }

  //-- other methods --------------------------------------------------------------------------
  int robotMotor::getID() {
    return motorID;
  }

  void robotMotor::printPosition() {
    Serial.print("motor[");
    Serial.print(motorID);
    Serial.print("]:");
    Serial.println(position);
  }

