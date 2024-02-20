#include "../../include/subsystems/Actuation.hpp"

Actuation::Actuation()
{
    this->actuation_motor.Config_kP(0,ACTUATION_P);
    this->actuation_motor.Config_kI(0,ACTUATION_I);
    this->actuation_motor.Config_kD(0,ACTUATION_D);
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}

void Actuation::actuate_to_point(float point)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position,(point/360)*TALONFX_UFR);
    }
}

void Actuation::linear_actuation(float input)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
    }
}

void Actuation::Periodic()
{
    /* Smartdashboard locked status */
}