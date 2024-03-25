#pragma once

#include "../Constants.hpp"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix6/TalonFX.hpp>

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

    private:
        enum Climb_State
        {
            Retracted,
            Extending,
            Extended,
            Retracting
        };

        /// @brief The intake state.
        Climb_State m_state = Retracted;

        /// @brief The climber right side motor.
        TalonFX m_climb_motor_right{CLIMBER_RIGHT_CAN_ID, CANBUS_NAME};

        /// @brief The climber left side motor.
        TalonFX m_climb_motor_left{CLIMBER_LEFT_CAN_ID, CANBUS_NAME};

        /// @brief Array to hold pointers to the climber motors
        TalonFX *m_climb_motors[2] = 
        {
            &m_climb_motor_right,
            &m_climb_motor_left
        };

        /// @brief Method to set the climb hooks to the specified position.
        /// @param position - The position to set the climb hooks.
        void Set_Position(float position);
};
