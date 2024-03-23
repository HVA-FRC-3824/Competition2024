#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/Actuation.hpp"

/// @brief Constructor for the Actuation class.
Actuation::Actuation()
{
    // Configure the actuate motors
    this->m_actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    this->m_actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::CTRE_MagEncoder_Absolute,0,0);
}

/// @brief Method called periodically every operator control packet.
void Actuation::Robot_Periodic()
{
    // Show the acturate state
    frc::SmartDashboard::PutNumber("Actuation Angle: ",  (this->m_actuation_motor.GetSelectedSensorPosition() / 806400) * 360);
    frc::SmartDashboard::PutNumber("Actuation Pos: ",     this->m_actuation_motor.GetSelectedSensorPosition());
    frc::SmartDashboard::PutBoolean("Actuation Locked? ", this->actuation_locked);
}

/// @brief Method to move the actuate sybsystem to the specified angle.
/// @param angle - The angle to set the actuate subsystem.
void Actuation::Actuate_To_Point(float angle)
{
    // Determine if the user is allows to control the actutate subsystem
    if (!actuation_locked)
    {
        // Set tehe actuate subsystem to the specified angle
        frc::SmartDashboard::PutNumber("Actuation Desired: ", (angle / 360) * 806400);
        this->m_actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, (angle / 360) * 806400);
    }
}

/// @brief Method to set the actuate subsystem motor to the specifed set value.
/// @param motor_set_value - The motor set value for the actuate motor.
void Actuation::Linear_Actuation(float motor_set_value)
{
    // Determine if the user is allows to control the actutate subsystem
    if (!actuation_locked)
    {
        /*if((this->m_actuation_motor.GetSelectedSensorPosition()/2048)*360 < MAX_LOWER_ANGLE)
        {return;} */

        // Set tehe actuate motor to the specified set value
        this->m_actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, motor_set_value);
    }
}
