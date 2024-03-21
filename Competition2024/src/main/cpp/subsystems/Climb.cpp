#include "../../include/subsystems/Climb.hpp"

// Conctructor for the Climb Subassembly, which instantiates the tape winch motor
// and the climber motor
Climb::Climb()
{
    DEPLOY_MOTOR_RIGHT.SetNeutralMode(motorcontrol::Coast);
    DEPLOY_MOTOR_LEFT.SetNeutralMode(motorcontrol::Brake);
}

// Set the climb motor power (-1.0 to 1.0)
void Climb::climb(float input)
{
    
    DEPLOY_MOTOR_RIGHT.Set(input);
}