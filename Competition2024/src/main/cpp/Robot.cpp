// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <fmt/core.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <iostream>

#include "../include/io/OperatorController.hpp"
#include "../include/io/CommandHelper.hpp"
#include "../include/io/DriverController.hpp"
#include "../include/subsystems/Launcher.hpp"
#include "../include/subsystems/Swerve.hpp"
#include "../include/subsystems/Autos.hpp"
#include "../include/subsystems/Climb.hpp"
#include "../include/Memory.h"

/* Object and struct declaration */
cmd_share       cmd_control;
angle_mem_share angles_share;
Launcher        LAUNCHER{};
Intake          INTAKE{};
Actuation       ACTUATION{};
Climb           CLIMB{};

Swerve SWERVE{24,24};

OperatorController O_CONTROLLER{&cmd_control,&LAUNCHER,&INTAKE,&ACTUATION, &CLIMB}; 
DriverController   D_CONTROLLER{&SWERVE};

Autos AUTOS{&SWERVE,&cmd_control,&ACTUATION};

frc::Joystick Jostick{0};
uint8_t blocked_tags[8] = {8,9,10,11,12,13,14,15};


void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameCustom, kAutoNameCustom);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  m_chooser.AddOption(kShootMoveAuto,kShootMoveAuto);
  m_chooser.AddOption(kMobilityAuto, kMobilityAuto);
  m_chooser.AddOption(kShootMoveLeftAuto, kShootMoveLeftAuto);
  m_chooser.AddOption(kShootMoveRightAuto, kShootMoveRightAuto);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  //open_TheEye(THE_EYE,blocked_tags,8);
  //navx.Reset();
  runner_launcher(&cmd_control,&O_CONTROLLER,&AUTOS);
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
  //SWERVE.snap_wheels_to_abs(); 
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  fmt::print("Auto selected: {}\n", m_autoSelected);

  if (m_autoSelected == kAutoNameCustom) {
    AUTOS.do_nothing_auto();
  } else if(m_autoSelected == kShootMoveAuto) {
    AUTOS.shoot_then_move();
  } else if (m_autoSelected == kMobilityAuto) {
    AUTOS.mobility();
  } else if (m_autoSelected == kShootMoveLeftAuto) {
    AUTOS.shoot_then_move_left();
  } else if (m_autoSelected == kShootMoveRightAuto) {
    AUTOS.shoot_then_move_right();
  }
}

void Robot::AutonomousPeriodic() {
  if(AUTOS.state == A_TRYING_FOR_ANGLE)
  {
    //SWERVE.drive(0,0,AUTOS.ANGLE_CONTROLLER.Calculate(SWERVE.navx.GetYaw()),0);
    frc::SmartDashboard::PutNumber("ANGLE CONTROLLER OUTPUT: ",AUTOS.ANGLE_CONTROLLER.Calculate(SWERVE.navx.GetYaw()));
    if(AUTOS.ANGLE_CONTROLLER.AtSetpoint()){AUTOS.state = A_FINISHED;}
  }
}

void Robot::TeleopInit()
{
  SWERVE.snap_wheels_to_abs();
  cmd_control.my_wishes = C_KILL; // kill existing auto command if hung
}

int timer = 0;

void Robot::TeleopPeriodic() {
  O_CONTROLLER.robo_periodic(); /* Operator Periodic */
  D_CONTROLLER.robo_periodic(); /* Driver Periodic   */

  command_runner();
  INTAKE.robo_periodic();
  ACTUATION.robo_periodic();
  //angles_share.swerve_heading = navx.GetAngle();
  //std::cout << navx.GetAngle() << "\n";
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
