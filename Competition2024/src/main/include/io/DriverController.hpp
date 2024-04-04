#pragma once

#include <frc/Joystick.h>

#include "../Constants.hpp"
#include "../subsystems/Swerve.hpp"

class DriverController
{
    public:

        /// @brief Constructor for the DriverController class.
        /// @param swerve - Pointer to the swerve drive class.
        DriverController(Swerve *swerve);

        /// @brief Method called periodically every dirver/operator control packet.
        void Robot_Periodic();

    private:

        /// @brief Method to create dead zones for the controller joysticks and to have finer control.
        /// @param x - Pointer to the x stick value to return the value used.
        /// @param y - Pointer to the y stick value to return the value used.
        /// @param x2 - Pointer to the second x stick value to return the value used.
        void correct_inputs(float *x, float *y, float *x2);

        /// @brief Pointer to the swerver drive class.
        Swerve *m_swerve;

        /// @brief The driver controller (joystick).
        frc::Joystick m_driver_joystick{DRIVER_CONTROLLER};
        frc::XboxController m_grahams_special_stick{0}
};
