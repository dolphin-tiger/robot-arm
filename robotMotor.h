/****************************************************************************************************
  @file robotMotor.h
  @brief Servo Motor control class utilizing pwm module
  @author Jeremy Reynolds <jeremy.reynolds.123@gmail.com>
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

#ifndef robotMotor_h
#define robotMotor_h
//#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>

  #include <Arduino.h>

  class robotMotor {
    private:

      /**
        @brief parameters for motor number (pwm channel) and i2c address of controller

        @details
        These parameters capture what controller address and motor number.
        The i2c address is defaulted to HEX 0x40 but can be modified on the controller 
        board. It is also possible to use more than one controller board with this 
        class to control more motors.  The motor ID is the pwm channel of the 
        controller that is being used for the motor object.
      */
      int i2cAddress = 0x40;
      int motorID;

      /**
        @brief parameters for default position and startup position of motor

        @details
        These are the parameters that define the default positions and starup of the 
        motor when it attaches.  The servos have a range of 0 to 180 degrees.
        These parameters define that.
      */
      int minPosition = 0;
      int maxPosition = 180;
      int centerPosition = 90;
      int position = 90;

      /**
        @brief define the controller board for the motor

        @details
        Controller is defined here with the i2c address. This initialization needs 
        to be rethought and placed in the correct scope.  This works as is but is 
        not the correct place.
      */
      Adafruit_PWMServoDriver svo = Adafruit_PWMServoDriver(i2cAddress);

    public:

      /**
      @brief Class constructor. Create a new object of the robotMotor
      */
      robotMotor();

      /**
      @brief method to connect the motor to the pwm channel
      @param i2c (i2c address of controller board)

      */
      void attach(int i2c, int ch);

      /**
      @brief method to move the motor incrementally.
      @details
      This method moves the motor relative to it's current position. A 
      positive value will move the motor counter-clockwise by that number 
      of degrees, and a negative number will move it clockwise.  The motor 
      will be constrained to the min and max positions defined at runtime.
      @param i2c (i2c address of controller board that the motor is connected to.)
      @param ch (channel of controller board motor that the motor is connected to.)
      */
      void moveInc(int val);

      /**
      @brief methods to move the motor to a specific position.
      @details
      This method moves the motor to an absolute position. The 
      motor can be set to any value between min and max positions. 
      If it is set to less than min or greater than max it will go 
      to those respective min or max positions.
      */
      void setPosition(int pos);

      /**
      @brief methods to get the current position of the motor.
      @details
      This method returns the current absolute position of the motor.
      */
      int getPosition();

      /**
      @brief methods to set a center position of the motor.
      @details
      This method is not used for much but does allow a value to be set and 
      recalled at runtime.  It is also used at startup to set the starting 
      position of the motor.
      */
      void setCenterPosition(int pos);

      /**
      @brief method to get a center position of the motor.
      @details
      This method will report the stored center position. This can be used 
      with setPosition to recall the motor back to it's startup position.
      */
      int getCenterPosition();

      /**
      @brief methods to set a minimum position of the motor.
      @details
      This method is used to set or get a minimum position of the motor. 
      All methods that move the motor will follow these constraints so that 
      the motor will not move to a value less than the minimum position.
      */
      void setMinPosition(int pos);

      /**
      @brief methods to get the stored minimum position of the motor.
      @details
      This method is used to get the minimum position of the motor. 
      All methods that move the motor will follow these constraints so that 
      the motor will not move to a value less than the minimum position.  
      */
      int getMinPosition();

      /**
      @brief methods to set a maximum position of the motor.
      @details
      This method is used to set a maximum position of the motor. 
      All methods that move the motor will follow these constraints so that 
      the motor will not move to a value more than the maximum position.
      */
      void setMaxPosition(int pos);

      /**
      @brief methods to get the stored maximum position of the motor.
      @details
      This method is used to get the stored maximum position of the motor. 
      All methods that move the motor will follow these constraints so that 
      the motor will not move to a value more than the maximum position.
      */
      int getMaxPosition();

      /**
      @brief methods to get the motor id which is pwm channel
      */
      int getID();

      /**
      @brief method to print the current position of the motor to serial monitor
      */
      void printPosition();
  };

#endif