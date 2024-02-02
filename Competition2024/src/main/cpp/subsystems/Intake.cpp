#include "../../include/subsystems/Intake.hpp"

void Intake::suck(float input)
{
    this->intake_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
};