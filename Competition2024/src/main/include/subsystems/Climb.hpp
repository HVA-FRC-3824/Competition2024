#pragma once

#include "../Constants.hpp"
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix6/TalonFX.hpp>

using namespace ctre::phoenix::motorcontrol::can;

class Climb
{
    public:
        /// @brief Constructor for the Climb class.
        Climb();
        
        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Cliber motor controller
        /// @param motor_set_value - The motor set value (-1.0 to 1.0).
        void Climber_Control(float motor_set_value);

    private:

        /// @brief The climber right side motor.
        WPI_TalonSRX m_deploy_motor_right{CLIMBER_DEPLOY_RIGHT_CAN_ID};

        /// @brief The climber left side motor.
        WPI_TalonSRX m_deploy_motor_left{CLIMBER_DEPLOY_LEFT_CAN_ID};
};
