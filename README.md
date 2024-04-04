# 3824 2024 Crescendo
3824's Robot Code for the 2023-2024 First Robotics Competition Season "Crescendo"

Developed by:
Luke Smith, Jackson Case, and Mr.Young

"Repository in eterenal turmoil." 
  - Dr. Maples

## How does this work?

This repo uses [WPIlib](https://docs.wpilib.org/en/stable/index.html), [Rev Robotics](https://www.revrobotics.com/), [CTRE](https://store.ctr-electronics.com/), and more to make our robot work. As a base, we used WPIlib to create a template for our robot, and this provided the general structure of it. The robot is divided into sections called "subsystems" that drive individual parts of the robot, eg: "Swerve", "Intake", "Launcher", etc. These subsystems are pulled together by a a main "Robot.cpp" file that runs functions designated mainly by WPIlib. Then, the I/O is mainly handled in the aptly named "io" directory, where we point controller input into the robot subsystems.

## Most importantly, What are the controls?
This code uses 2 controllers, and operator and a driver. obviously one just does swerve, and the other one does everything else. 

### Operator
**climb**
Bumpers: raises respecive climbers
Triggers: lowers respective climbers
b-button: raises intake into an upwards position thats out of the way.
NOTE: bumbers and triggers can reverse if pushed all the way, due to the winch functionality.
**intake movement**
d-pad:
  up: source position
  down: amp position     
  left: turn off motors aka start position (all the way down)
  right: turn on motors
  NOTE: get into the amp first in source position then move to amp.
b-button: raises intake into an upwards position, out of the way for climb.
x-button: flip between amp and source
y-button: add intake offset
**intake rollers**
left joystick up: blow
left joystick up: suck

### Driver
***IMPORTANT***
after auto align yourself straight-forwards and press A
a-button: resets field centricity
**movement**
left joystick: standard up;forward side-to-side;strafe
right joystick: side-to-side does turning
right bumper: toggles faster or slower movement (30% of max)
**defense**
b-button: toggles x-wheels, this puts the wheels into an X so that it can't move despite being pushed.
