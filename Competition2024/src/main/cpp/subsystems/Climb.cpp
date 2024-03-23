#include "../../include/subsystems/Climb.hpp"

/// @brief Constructor for the Climb class.
Climb::Climb()
{
    // Configure the climber motors
    m_deploy_motor_right.SetNeutralMode(motorcontrol::Coast);
    m_deploy_motor_left.SetNeutralMode(motorcontrol::Brake);
}

/// @brief Method called periodically every operator control packet.
void Climb::Robot_Periodic()
{

}

/// @brief Cliber motor controller
/// @param motor_set_value - The motor set value (-1.0 to 1.0).
void Climb::Climber_Control(float motor_set_value)
{
    // Set the climber motor outputs
    m_deploy_motor_right.Set(motor_set_value);
    m_deploy_motor_left.Set(motor_set_value);
}
