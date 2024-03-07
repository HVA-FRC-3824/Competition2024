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
        void winch(float input);
    private:
        WPI_TalonSRX DEPLOY_MOTOR{CLIMBER_DEPLOY};
        ctre::phoenix6::hardware::TalonFX WINCH_MOTOR{CLIMBER_WINCH,"rio"};
};


#endif