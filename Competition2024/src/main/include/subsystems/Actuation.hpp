#pragma once

#include "../Constants.hpp"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>

using namespace ctre::phoenix::motorcontrol::can;

class Actuation : frc2::SubsystemBase
{
    public:
        /// @brief Boolean to lock out the operator from the actuation subsystem.
        bool actuation_locked = false;

        /// @brief Constructor for the Actuation class.
        Actuation();

        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Method to move the actuate sybsystem to the specified angle.
        /// @param angle - The angle to set the actuate subsystem.
        void Actuate_To_Point(float angle);

        /// @brief Method to set the actuate subsystem motor to the specifed set value.
        /// @param motor_set_value - The motor set value for the actuate motor.
        void Linear_Actuation(float motor_set_value);

    private:

        /// @brief Motor for the actuation subsystem.
        WPI_TalonSRX m_actuation_motor{ACTUATION_CAN_ID};
};
