// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <fmt/core.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include <iostream>
#include "AHRS.h"
#include "../include/io/OperatorController.hpp"
#include "../include/io/CommandHelper.hpp"
#include "../include/io/DriverController.hpp"
#include "../include/subsystems/Turret.hpp"
#include "../include/subsystems/TheEye.h"
#include "../include/subsystems/Launcher.hpp"
#include "../include/subsystems/Swerve.hpp"
#include "../include/Memory.h"

/* Object and struct declaration */
cmd_share cmd_control;
angle_mem_share angles_share;
Turret TURRET{&angles_share};
Launcher LAUNCHER{};
//struct TheEye THE_EYE;
/* Test */
Turret *t_hold;
Launcher *l_hold;
Intake INTAKE{};
Actuation ACTUATION{};

AHRS navx{frc::SerialPort::SerialPort::Port::kMXP};
Swerve SWERVE{24,24,&navx};

OperatorController O_CONTROLLER{&cmd_control,&TURRET,&navx,&LAUNCHER,&INTAKE,&ACTUATION}; 
DriverController D_CONTROLLER{&SWERVE};


frc::Joystick Jostick{0};
uint8_t blocked_tags[8] = {8,9,10,11,12,13,14,15};


void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  //open_TheEye(THE_EYE,blocked_tags,8);
  navx.Reset();
  runner_launcher(&cmd_control,&O_CONTROLLER);
}

/**
 * This function is called every 20 ms, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() { /*print_data(tag);*/ }

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  fmt::print("Auto selected: {}\n", m_autoSelected);

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {  }

int timer = 0;

void Robot::TeleopPeriodic() {
  O_CONTROLLER.robo_periodic(); /* Operator Periodic */
  //D_CONTROLLER.robo_periodic(); /* Driver Periodic   */

  command_runner();
  //INTAKE.robo_periodic();
  //TURRET.spin_to_angle(75);
  //TURRET.robo_periodic();
  //ACTUATION.robo_periodic();
  angles_share.swerve_heading = navx.GetAngle();
  //std::cout << navx.GetAngle() << "\n";
  /* if((tag.center_x <= 380 && tag.center_x >= 280))
  {
    TURRET.spin_simple(0); // lock in place 
  }
  if(timer = 1)
  {
    TURRET.spin_simple(0);
  }
  if(tag.center_x < 280){TURRET.spin_simple(.08); timer++;}
  if(tag.center_x > 380){TURRET.spin_simple(-.08); timer++;} */

  
  //LAUNCHER.simple_spin(Jostick.GetY());
}

void Robot::DisabledInit() { }

void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}

void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}

void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
