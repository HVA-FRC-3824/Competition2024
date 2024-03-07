#include "../../include/subsystems/Climb.hpp"

Climb::Climb()
{
    DEPLOY_MOTOR.SetNeutralMode(motorcontrol::Coast);
    WINCH_MOTOR.SetNeutralMode(motorcontrol::Brake);
}

void Climb::climb(float input)
{
    
    DEPLOY_MOTOR.Set(input);
}

void Climb::winch(float input)
{
    WINCH_MOTOR.Set(input);
}