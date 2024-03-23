#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/Actuation.hpp"

using namespace ctre::phoenix::motorcontrol;

/// @brief Constructor for the Actuation class.
Actuation::Actuation()
{
    // Configure the actuate motors
    this->m_actuation_motor.SetNeutralMode(NeutralMode::Brake);
    //this->m_actuation_encoder.ConfigSelectedFeedbackSensor(FeedbackDevice::, 0, 0);
}


/// @brief Method called periodically every operator control packet.
void Actuation::Robot_Periodic()
{

    // Reaad encoder
    int encoder_position = this->m_actuation_encoder.GetSelectedSensorPosition(0);

    // Converting encoder position to angle to angle
    double angle = encoder_position * (90 / (A_VERTICAL - A_HORIZONTAL)) + A_HORIZONTAL;

    // Run PID Controller
    double pid_calculation = this->actuation_PID_controller.Calculate(ACTUATION_SETPOINT, ACTUATION_PRESENT);

    // Set motor out put.
    m_actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Velocity, pid_calculation);
    
    // Show the acturate state
    frc::SmartDashboard::PutNumber("Actuation Angle: ",  angle);
    frc::SmartDashboard::PutNumber("Actuation Encoder Position: ", encoder_position);
    //frc::SmartDashboard::PutBoolean("Actuation Locked? ", this->actuation_locked);
}

/// @brief The Method to set the desired angle for the actuation subsystem.
/// @param angle - The angle to set the actuation to.
void Actuation::Set_Angle(double angle) 
{
    m_angle = angle;
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
