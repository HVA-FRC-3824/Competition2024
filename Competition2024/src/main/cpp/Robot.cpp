// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <iostream>

#include "Robot.hpp"

/// @brief Called when the robot is started.
void Robot::RobotInit()
{
    m_chooser.SetDefaultOption(kAutoNameCustom, kAutoNameCustom);
    m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
    m_chooser.AddOption(kAuto_MoveAuto, kAuto_MoveAuto);

    frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

    // We need to run our vision program in a separate thread. If not, our robot
    // program will not run.
    std::thread visionThread(VisionThread);
    visionThread.detach();
}

// This function is called every 20 ms, no matter the mode. Use
// this for items like diagnostics that you want ran during disabled,
// autonomous, teleoperated and test.
// 
//  <p> This runs after the mode specific periodic functions, but before
//  LiveWindow and SmartDashboard integrated updating.
void Robot::RobotPeriodic()
{ 

}

// This autonomous (along with the chooser code above) shows how to select
// between different autonomous modes using the dashboard. The sendable chooser
// code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
// remove all of the chooser code and uncomment the GetString line to get the
// auto name from the text box below the Gyro.
//
// You can add additional auto modes by adding additional comparisons to the
// if-else structure below with additional strings. If using the SendableChooser
// make sure to add them to the chooser code above as well.
void Robot::AutonomousInit()
{
    // Set the wheels to absolute position
    SWERVE.Snap_Wheels_To_Absolute_Position();

    // Choose the autonomous command
    m_AutoCommandselected = m_chooser.GetSelected();

    // m_AutoCommandselected = SmartDashboard::GetString("Auto Selector", kAutoNameDefault);
    fmt::print("Auto selected: {}\n", m_AutoCommandselected);

    // Set the autonomous command
    if (m_AutoCommandselected == kAutoNameCustom)
        Autonomous.Auto_Do_Nothing();
    else if (m_AutoCommandselected == kAuto_MoveAuto)
        Autonomous.Auto_Move();
}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{
    // Set the wheels to absolute position
    SWERVE.Snap_Wheels_To_Absolute_Position();
}

void Robot::TeleopPeriodic()
{
    // Get the operator and driver inputs
    O_CONTROLLER.Robot_Periodic();
    D_CONTROLLER.Robot_Periodic();

    // Run the subassembly periodic methods
    INTAKE.Robot_Periodic();
    CLIMB.Robot_Periodic();
}

void Robot::DisabledInit()
{

}

void Robot::DisabledPeriodic()
{

}

void Robot::TestInit()
{

}

void Robot::TestPeriodic()
{

}

void Robot::SimulationInit()
{

}

void Robot::SimulationPeriodic()
{

}

#ifndef RUNNING_FRC_TESTS
int main()
{
    return frc::StartRobot<Robot>();
}
#endif
