# robot-arm
We branded/named the project the **VE2**.

![logo-ve2-invert-sm](https://github.com/dolphin-tiger/robot-arm/assets/62484970/2362d84b-6f2c-45c0-a60b-b1bd6310ce55)



https://github.com/dolphin-tiger/robot-arm/assets/62484970/2833d79a-bbf1-4e61-a818-328a371b3459


robot-arm is a 5th graders class project to build and control a robotic arm utilizing an Arduino controller, servo motors, and analog joysticks. The original goal of the project was to learn how coding works, with the specific request to start from scratch as opposed to modifying something existing.  We mostly accomplished that task here by creating our own ncludes but also learned why at some point it is impractical and undesirable to recreate all the code from scratch. This project has a good mixture of scratch code along with libraries like from Adafruit and other even more obvious like <Arduino.h>.

We got as far as building the arm as of the time of writing this.  We have not built or integrated the claw yet.

## Design
We used [TinkerCAD](https://www.tinkercad.com/) to model the components.
[![image](https://github.com/dolphin-tiger/robot-arm/assets/62484970/c8daf28d-fe84-48b7-81ff-2887e7d1dad4)](https://www.tinkercad.com/things/9WjpDJQ0Wra-ve2-full-design)

This is a mock simulation built in TinkerCAD.  TinkerCAD doesn't have all of the components to fully build our project, but this was a good place to start with basic wiring and control.  The potentiometers are used in place of the joysticks to simulate.  We will work on adding additional wiring information to this readme.
![VE2 - Simulation Code](https://github.com/dolphin-tiger/robot-arm/assets/62484970/d8affc59-0c05-46d3-bbf1-900d47e387c8)


## Parts
- (1) [Arduino Mega 2560 Rev3](https://store.arduino.cc/products/arduino-mega-2560-rev3) - Controller (could use an [Arduino Uno](https://store-usa.arduino.cc/collections/boards-modules/products/arduino-uno-rev3?_pos=8&_fid=24682fd5b&_ss=c))
- (3) [DSSERVO RDS3218](https://www.dsservo.com/en/show_down.asp?id=29) - Servo Motors 20Kg
- (1) [Adafruit PCA9685](https://www.adafruit.com/product/815) - 16ch PWM Controller
- (2) [PS2 Joystick](http://www.hiletgo.com/ProductDetail/1953020.html) - Analog Joystick
- (1) 5VDC 10A Power Supply

## Custom Parts
**Base:** [TinkerCAD](https://www.tinkercad.com/things/jKUhlaCRjJ6-ve2-base) - The base is made out of 5mm plexiglass that was cut by a friend with a laser cutter.  From TinkerCAD you just export a .svg file of the object that will import directly into the laser cutter software.
![image](https://github.com/dolphin-tiger/robot-arm/assets/62484970/d768acec-bae4-4f7b-821b-711e57bfaee3)

**Joystick Controller:** [TinkerCAD](https://www.tinkercad.com/things/77tcLyRLhxv-ve2-controller)
![image](https://github.com/dolphin-tiger/robot-arm/assets/62484970/450e1612-b572-44b9-bbd5-e902b02e7b78)


**Motor Mount:** [TinkerCAD](https://www.tinkercad.com/things/eI32lApy8ZL-ve2-motor-mount) - The motor mount is a 3D printed part to attach the bottom motor to the base plate.  This was printed by another friend with a 3D printer.

![image](https://github.com/dolphin-tiger/robot-arm/assets/62484970/a4599940-fdb5-4ee8-8adc-d7b004279810)
