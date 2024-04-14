/****************************************************************************************************
  @file joystick.cpp
  @brief Joystick class with center calibration
  @author Jeremy Reynolds <jeremy.reynolds.123@gmail.com>
  @version 1.0.1
  @date 2024/03/22

  @details
  Joystick is a simple class written in Arduino IDE to adjust for some common needs in simple analog 
  joysticks.  It assumes an XY dual axis joystick and will allow calibating the center point of the joystick
  at runtime. just call the .calibrateCenter() method while the joystick is in the neutral center position.
  This will capture the center point and adjust the mapping to accomidate.

  version 1.0.1 - added deadband around mid point so the joystick doesn't drift at rest.
  
  # LICENSE #
  
  MIT License

  Copyright (c) 2024 Jeremy Reynolds

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
#include "joystick.h"
#include <Arduino.h>

joystick::joystick(uint16_t x, uint16_t y, uint16_t b) {
  //--- assign pins being used
  x_pin = x;
  y_pin = y;
  b_pin = b;

  //--- set the pinMode for each pin
  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(b_pin, INPUT_PULLUP);

  b_state = digitalRead(b_pin);
}

/**
  @brief method to set the center point of the joystick when at neutral position
  @details
  calibrateCenter() will capture the centerpoint of the joystick.  While the 
  joystick is in neutral position, call this method to capture the center value
  for both the X and Y axis.  The getPosition() method will use these values to
  report back an adjusted range with MID(512) at the neutral position.
*/
void joystick::calibrateCenter() {
  uint16_t x; uint16_t y;

  x = analogRead(x_pin);
  if(x > mid_min && x < mid_max) { x_mid = x; }

  y = analogRead(y_pin);
  if(y > mid_min && y < mid_max) { y_mid = y; }

  Serial.print("\njoystick.h\nCenter calibration: x_mid:"); Serial.print(x_mid); Serial.print(", y_mid:"); Serial.println(y_mid);
}

int16_t joystick::getPos(axis_t axis) {
  int16_t val;
  int16_t axisMin;
  int16_t axisMid;
  int16_t axisMax;

  switch(axis) {

    case X: {
      //--- read the value from the X pin
      val = analogRead(x_pin);

      //--- setup values for the x parameters
      axisMin = x_min;
      axisMid = x_mid;
      axisMax = x_max;

      //--- print debug info if enabled
      if(debug==true) { Serial.print("debug X="); Serial.print(val); Serial.print("time:"); Serial.print(millis()); Serial.println(); }
      
      //--- break the switch case statement
      break;
    }

    case Y: {
      //--- read the value from the Y pin
      val = analogRead(y_pin);

      //--- setup values for the Y parameters
      axisMin = y_min;
      axisMid = y_mid;
      axisMax = y_max;

      //--- print debug info if enabled
      if(debug==true){Serial.print("debug y="); Serial.print(val); Serial.print("time:"); Serial.print(millis()); Serial.println();}
      
      //--- break the switch case statement
      break;
    }

  }

  //--- if the value is near the mid point then return the ideal mid point instead.
  //--- this creates a deadband near the mid point so it doesn't drift.
  if( abs(val - axisMid) <= mid_deadband ) {
    val = mid;
  }

  //--- otherwise, if the value is below the calibrated mid point then scale the value to the ideal range min - mid.
  else if(val < axisMid) {
    val = map(val, axisMin, axisMid, min, mid);
  }

  //--- otherwise, the value is above the calibrated mid point so scale the value to the ideal range mid - max.
  else { 
    val = map( val, axisMid, axisMax, mid, max );
  }

  return val;
}

/**
  @brief method to report the position of the joystick adjusted to the calibrated center position
  @details
  getPosition() will report the position of the joystick for either X or Y axis specified. The 
  calibrated center point will be adjusted for when the value is reported back.
*/
int16_t joystick::getPosition(axis_t axis) {
  
  //--- return the value
  return getPos(axis);
}

/**
  @brief method to report the position of the joystick adjusted to the calibrated center position then scaled to range min/max
  @details
  getPosition() will report the position of the joystick for either X or Y axis specified, scaled to the supplied rangeMin/Max 
  values. The calibrated center point will be adjusted for when the value is reported back.
*/
int16_t joystick::getPosition(axis_t axis, int16_t rangeMin, int16_t rangeMax, bool invert = false){

  if(invert){

    int r1 = rangeMin;
    int r2 = rangeMax;
    rangeMin = r2;
    rangeMax = r1;

  }

  return map( getPos(axis), min, max, rangeMin, rangeMax);

}

/**
  @brief method to report the state of the button release
  @details
  getButton() will report the state of the joystick button release.  This will also capture the current
  state to compare the next call against.  This function will return true when the button is released.
*/
bool joystick::getButton(){

  bool b_val = digitalRead(b_pin);
  if(b_state != b_val) {
    b_state = b_val;
    if(b_state == true) {
      return true;
    }
  }
  else {
    return false;
  }

}