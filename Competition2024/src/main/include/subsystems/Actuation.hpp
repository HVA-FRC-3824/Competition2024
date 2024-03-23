#pragma once

#include "../Constants.hpp"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <frc/controller/PIDController.h>

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

        /// @brief The Method to set the desired angle for the actuation subsystem.
        /// @param angle - The angle to set the actuation to.
        void Set_Angle(double angle);

    private:

        /// @brief The desired angle for the Actuation.
        double m_angle;

        /// @brief Motor for the actuation subsystem.
        WPI_TalonFX m_actuation_motor{ACTUATION_MOTOR_CAN_ID};

        /// @brief Encoder for the actuation subsystem.
        WPI_TalonFX m_actuation_encoder{ACTUATION_ENCODER_CAN_ID};

        /// @brief The period between controller updates in seconds. It is 20 milliseconds.
        units::second_t period = 20_ms;


        //ctre::phoenix::motorcontrol::ControlMode Velocity;//actuation_control_mode;

        /// @brief The PID controller for the actuation subsystem.
        frc::PIDController actuation_PID_controller{INTAKE_ACTUATION_P, INTAKE_ACTUATION_I, INTAKE_ACTUATION_D, period};
};
