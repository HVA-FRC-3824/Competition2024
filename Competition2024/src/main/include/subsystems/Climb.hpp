#ifndef CLIMB_H
#define CLIMB_H 
#include "../Constants.h"
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix6/TalonFX.hpp>

using namespace ctre::phoenix::motorcontrol::can;

class Climb
{
    public:
        Climb();
        void climb(float input);
    private:
        WPI_TalonSRX DEPLOY_MOTOR_RIGHT{CLIMBER_DEPLOY_RIGHT};
        WPI_TalonSRX DEPLOY_MOTOR_LEFT{CLIMBER_DEPLOY_LEFT};
};


#endif