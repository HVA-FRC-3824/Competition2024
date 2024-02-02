#ifndef INTAKE_H
#define INTAKE_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>

class Intake : frc2::SubsystemBase
{
    public:
        void suck(float input);
    private:
        ctre::phoenix::motorcontrol::can::WPI_TalonSRX intake_motor{INTAKE_MOTOR};
};

#endif