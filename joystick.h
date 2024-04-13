/****************************************************************************************************
  @file joystick.h
  @brief Joystick class with center calibration
  @author Jeremy Reynolds <jeremy.reynolds.123@gmail.com>
  @version 1.0.0
  @date 2024/03/22

  @details
  Joystick is a simple class written in Arduino IDE to adjust for some common needs in simple analog 
  joysticks.  It assumes an XY dual axis joystick and will allow calibating the center point of the joystick
  at runtime. just call the .calibrateCenter() method while the joystick is in the neutral center position.
  This will capture the center point and adjust the mapping to accomidate.

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

#ifndef joystick_h
#define joystick_h

  #include <Arduino.h>

  typedef enum axis:uint8_t {X=0, Y} axis_t;

  class joystick {
    private:
      /*
        @brief default parameters for the ideal min/mid/max range of the joystick

        @details
        These are the parameters that define the standard range of the joystick.
        Arduino analog pins read 0 - 1023 with 512 ideally being the center point.
        These parameters define that.
      */
      uint16_t min = 0;
      uint16_t mid = 512;
      uint16_t max = 1023;

      /*
        @brief define the allowable range for center calibration
        @details
        mid_min and mid_max define the allowable range for center calibration.
        If the calibration falls outside these values then it is not captured 
        and retains it's original value.
      */
      uint16_t mid_min = 256;
      uint16_t mid_max = 768;
      uint16_t mid_deadband = 100;
      /*
        @brief variables to hold the min/mid/max values for the X axis
        @details
        These variables are used to capture the min/mid/max values of the X axis.
        Currently the min/max values are fixed but a calibration process may be 
        added in the future to allow values that are not absolute min or max.
      */
      uint16_t x_pin;
      uint16_t x_min = min;
      uint16_t x_mid = mid;
      uint16_t x_max = max;
      
      /*
        @brief variables to hold the min/mid/max values for the Y axis
        @details
        These variables are used to capture the min/mid/max values of the Y axis.
        Currently the min/max values are fixed but a calibration process may be 
        added in the future to allow values that are not absolute min or max.
      */
      uint16_t y_pin;
      uint16_t y_min = min;
      uint16_t y_mid = mid;
      uint16_t y_max = max;

      /*
        @brief variables to hold the button values
        @details
        These variables are used to capture the min/mid/max values of the Y axis.
        Currently the min/max values are fixed but a calibration process may be 
        added in the future to allow values that are not absolute min or max.
      */
      uint16_t b_pin;
      bool b_state = false;   
      int16_t getPos(axis_t axis);

    public:
      bool debug = false;
      /**
      @brief Class constructor. Create a new object of the joystick and set the axes (X/Y) pin to input mode
      @param x (x analog port pin of the device)
      @param y (y analog port pin of the device)
      */
      joystick(uint16_t x, uint16_t y, uint16_t b);

      /**
        @brief method to report the position of the joystick adjusted to the calibrated center position
        @details
        getPosition() will report the position of the joystick for either X or Y axis specified. The 
        calibrated center point will be adjusted for when the value is reported back.
      */
      int16_t getPosition(axis_t axis);
      int16_t getPosition(axis_t axis, int16_t rangeMin, int16_t rangeMax, bool invert=false);

      /**
        @brief method to set the center point of the joystick when at neutral position
        @details
        calibrateCenter() will capture the centerpoint of the joystick.  While the 
        joystick is in neutral position, call this method to capture the center value
        for both the X and Y axis.  The getPosition() method will use these values to
        report back an adjusted range with MID(512) at the neutral position.
      */
      void calibrateCenter();
      bool getButton();
  };

#endif