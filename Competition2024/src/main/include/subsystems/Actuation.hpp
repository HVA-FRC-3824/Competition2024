#ifndef ACTUATION_H
#define ACTUATION_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>

using namespace ctre::phoenix::motorcontrol::can;

class Actuation : frc2::SubsystemBase
{
    public:
        Actuation();
        void actuate_to_point(float angle);
        void robo_periodic();
        void linear_actuation(float input);
        bool locked = false;
    private:
        WPI_TalonFX actuation_motor{ACTUATION_ID};
};


#endif