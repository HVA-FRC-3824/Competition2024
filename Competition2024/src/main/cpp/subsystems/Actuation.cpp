#include "../../include/subsystems/Actuation.hpp"

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