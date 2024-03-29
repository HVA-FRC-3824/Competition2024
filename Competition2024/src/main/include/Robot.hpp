// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <string>

#include <fmt/core.h>

#include <cameraserver/CameraServer.h>
#include <frc/TimedRobot.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/Joystick.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "../include/io/OperatorController.hpp"
#include "../include/io/DriverController.hpp"

#include "../include/subsystems/AutoCommands.hpp"

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
        // Robot subassemblies
        Swerve             SWERVE{CHASSIS_LENGTH, CHASSIS_WIDTH};
        Intake             INTAKE{};
        Climb              CLIMB{};

        // Robot controllers (operator and controller)
        OperatorController O_CONTROLLER{&INTAKE, &CLIMB};
        DriverController   D_CONTROLLER{&SWERVE};

        // Method to run autonomous commands
        AutoCommands       Autonomous{&SWERVE, &INTAKE};

        const std::string kAuto_Do_Nothing     = "Do Nothing";
        const std::string kAuto_Move           = "Auto Move";
        const std::string kAuto_Place_Amp_Red  = "Place Amp - Red";
        const std::string kAuto_Place_Amp_Blue = "Place Amp - Blue";

        // Sender for choosing the autonomous command.        
        frc::SendableChooser<std::string> m_chooser;
        std::string                       m_AutoCommandselected;

        static void VisionThread() 
        {
            // Get the USB camera from CameraServer
            cs::UsbCamera camera = frc::CameraServer::StartAutomaticCapture();

            // Set the resolution
            camera.SetResolution(640, 480);

            // Get a CvSink. This will capture Mats from the Camera
            cs::CvSink cvSink = frc::CameraServer::GetVideo();

            // Setup a CvSource. This will send images back to the Dashboard
            cs::CvSource outputStream = frc::CameraServer::PutVideo("Rectangle", 640, 480);

            // Mats are very memory expensive. Lets reuse this Mat.
            cv::Mat mat;

            while (true) 
            {
                // Tell the CvSink to grab a frame from the camera and put it in the
                // source mat. If there is an error notify the output.
                if (cvSink.GrabFrame(mat) == 0) 
                {
                    // Send the output the error.
                    outputStream.NotifyError(cvSink.GetError());

                    // skip the rest of the current iteration
                    continue;
                }

                // Put a rectangle on the image
                rectangle(mat, cv::Point(100, 100), cv::Point(400, 400), cv::Scalar(255, 255, 255), 5);

                // Give the output stream a new image to display
                outputStream.PutFrame(mat);
            }
        }
};
