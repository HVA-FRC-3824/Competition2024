#pragma once

#include "../Constants.hpp"

#include <frc2/command/SubsystemBase.h>

#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix6/TalonFX.hpp>
#include "ctre/Phoenix.h"

using namespace ctre::phoenix6::hardware;

class Climb : frc2::SubsystemBase
{
    public:
        /// @brief Constructor for the Climb class.
        Climb();
        
        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Method to extend the climb hooks.
        void Extend();

        /// @brief Method to retract the climb hooks.
        void Retract();

        /// @brief Method to set the motor output.
        /// @param output - The motor output percentage (-1.0 to 1.0)
        void Set_Motor_Output(double output);

        /// @brief Methos to enable the climber motors.
        /// @param state - The state of the motor enable 
        void Enable_Climber_Motors(bool state);

    private:
        enum Climb_State
        {
            Retracted,
            Extending,
            Extended,
            Retracting
        };

        /// @brief The intake state.
        Climb_State m_state = Extended;

        /// @brief Intdicates if the climber motor is enabled.
        bool climber_motor_enable[2] = { true, true };

        /// @brief The climber right side motor.
        TalonSRX m_climb_motor_right{CLIMBER_RIGHT_CAN_ID};

        /// @brief The climber left side motor.
        TalonSRX m_climb_motor_left{CLIMBER_LEFT_CAN_ID};

        /// @brief Array to hold pointers to the climber motors
        TalonSRX *m_climb_motors[2] = 
        {
            &m_climb_motor_right,
            &m_climb_motor_left
        };

        /// @brief Method to set the climb hooks to the specified position.
        /// @param position - The position to set the climb hooks.
        void Set_Position(double position);
};
