# 3824 2024 Crescendo
3824's Robot Code for the 2023-2024 First Robotics Competition Season "Crescendo"

Developed by:
Luke Smith, Jackson Case, and Mr.Young

"Repository in eterenal turmoil." 
  - Dr. Maples

## How does this work?

This repo uses [WPIlib](https://docs.wpilib.org/en/stable/index.html), [Rev Robotics](https://www.revrobotics.com/), [CTRE](https://store.ctr-electronics.com/), and more to make our robot work. As a base, we used WPIlib to create a template for our robot, and this provided the general structure of it. The robot is divided into sections called "subsystems" that drive individual parts of the robot, eg: "Swerve", "Intake", "Launcher", etc. These subsystems are pulled together by a a main "Robot.cpp" file that runs functions designated mainly by WPIlib. Then, the I/O is mainly handled in the aptly named "io" directory, where we point controller input into the robot subsystems.
