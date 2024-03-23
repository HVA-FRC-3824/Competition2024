// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <fmt/core.h>

#include <frc/Joystick.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "../include/io/OperatorController.hpp"
#include "../include/io/CommandScheduler.hpp"
#include "../include/io/DriverController.hpp"

#include "../include/subsystems/Swerve.hpp"
#include "../include/subsystems/AutoCommands.hpp"
#include "../include/subsystems/Climb.hpp"

class Robot : public frc::TimedRobot 
{
    public:
        void RobotInit()          override;
        void RobotPeriodic()      override;
        void AutonomousInit()     override;
        void AutonomousPeriodic() override;
        void TeleopInit()         override;
        void TeleopPeriodic()     override;
        void DisabledInit()       override;
        void DisabledPeriodic()   override;
        void TestInit()           override;
        void TestPeriodic()       override;
        void SimulationInit()     override;
        void SimulationPeriodic() override;

    private:
        // Command scheduler parameters
        CommandParameters  m_command_parameters;

        // Robot subassemblies
        Intake             INTAKE{};
        Actuation          ACTUATION{};
        Climb              CLIMB{};
        Swerve             SWERVE{24, 24};

        // Robot controllers (operator and controller)
        OperatorController O_CONTROLLER{&INTAKE, &ACTUATION, &CLIMB};
        DriverController   D_CONTROLLER{&SWERVE};

        // Method to run autonomous commands
        AutoCommands Autonomous{&SWERVE, &ACTUATION};

        const std::string kAutoNameDefault = "Default";
        const std::string kAutoNameCustom  = "Do Nothing Auto";
        const std::string kAuto_MoveAuto   = "Auto_Move";

        // Sender for choosing the autonomous command.        
        frc::SendableChooser<std::string> m_chooser;
        std::string                       m_AutoCommandselected;
};
