#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/Intake.hpp"

/// @brief Constructor for the Intake class.
Intake::Intake()
{
    //this->m_actuation_motor.Config_kP(0,INTAKE_ACTUATION_P);
    //this->m_actuation_motor.Config_kI(0,INTAKE_ACTUATION_I);
    //this->m_actuation_motor.Config_kD(0,INTAKE_ACTUATION_D);
    //this->m_actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor,0,0);
    //this->m_actuation_motor.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
    //this->m_actuation_motor.ConfigPeakOutputForward(.2);
    //this->m_actuation_motor.ConfigPeakOutputReverse(-.18);

    // configure the intake motors
    this->m_intake_angle_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
}

/// @brief Method called periodically every operator control packet.
void Intake::Robot_Periodic()
{
    //frc::SmartDashboard::PutNumber("Intake Pos: ", (this->m_actuation_motor.GetSelectedSensorPosition()));
    frc::SmartDashboard::PutBoolean("Retracted? :", this->retracted);
}

/// @brief Method to run the actuate motors.
/// @param motor_set_value - The actuate motor set value.
void Intake::Intake_Actuate_Simple(float motor_set_value)
{
    // Set the intake angle motor to the specified value
    this->m_intake_angle_motor.Set(motor_set_value);
}

/// @brief Method to set the actuate subassembly to the specified angle.
/// @param angle - The angle to set the acturate subassembly.
void Intake::Intake_Actuate_Point(float angle)
{
    //this->m_actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, (angle / 360) * INTAKE_ACTUATION_UFR);
    //this->m_actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, angle);
}

/// @brief Method to flip the intake subassembly from extend to retracted.
void Intake::Flip_Retraction()
{
    // Determine if the intake is retracted
    if (retracted)
    {
        std::cout << "non retracted\n";

        // Intake should now be extended
        retracted = false;

        // Set the intake angle to extended
        Intake_Actuate_Point(INTAKE_BOTTOM_POINT);
    } 
    else 
    {
        std::cout << "retracted\n";

        // Intake should now be retracted
        retracted = true;

        // Set the intake angle to retracted
        Intake_Actuate_Point(0);
    }
}
