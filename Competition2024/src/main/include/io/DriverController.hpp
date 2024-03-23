#pragma once

#include "../Constants.hpp"
#include "../subsystems/Swerve.hpp"
#include <frc/Joystick.h>

class DriverController
{
    public:
        /// @brief Constructor for the DriverController class.
        /// @param swerve - Pointer to the swerve drive class.
        DriverController(Swerve *swerve);

        /// @brief Method called periodically every dirver/operator control packet.
        void Robot_Periodic();

    private:
        /// @brief Pointer to the swerver drive class.
        Swerve *m_swerve;

        /// @brief The driver controller (joystick).
        frc::Joystick m_driver_joystick{DRIVER_CONTROLLER};
};
