#ifndef ACTUATION_H
#define ACTUATION_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>

using namespace ctre::phoenix::motorcontrol::can;

class Actuation : frc2::SubsystemBase
{
    public:
        void actuate_to_point(float angle);
        void Periodic() override;
        void linear_actuation(float input);
    private:
        WPI_TalonFX actuation_motor{ACTUATION_ID};
        bool locked = false;
};


#endif