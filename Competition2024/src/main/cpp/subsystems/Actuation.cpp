#include "../../include/subsystems/Actuation.hpp"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

Actuation::Actuation()
{
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    this->actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::CTRE_MagEncoder_Absolute,0,0);
}

void Actuation::actuate_to_point(float point)
{
    if(!locked)
    {
        frc::SmartDashboard::PutNumber("Actuation Desired: ", (point/360)*806400);
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position,(point/360)*806400);
    }
}

void Actuation::linear_actuation(float input)
{
    if(!locked)
    {
        /*if((this->actuation_motor.GetSelectedSensorPosition()/2048)*360 < MAX_LOWER_ANGLE)
        {return;} */
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
    }
}

void Actuation::robo_periodic()
{
    frc::SmartDashboard::PutNumber("Actuation Angle: ", (this->actuation_motor.GetSelectedSensorPosition()/806400)*360);
    frc::SmartDashboard::PutNumber("Actuation Pos: ", this->actuation_motor.GetSelectedSensorPosition());
    frc::SmartDashboard::PutBoolean("Actuation Locked? ", this->locked);
}