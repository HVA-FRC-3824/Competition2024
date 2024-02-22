#ifndef INTAKE_H
#define INTAKE_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>

class Intake : frc2::SubsystemBase
{
    public:
        Intake();
        void intake_actuate_simple(float input);
        void intake_actuate_point(float angle);
        void suck(float input);
        void robo_periodic();
        void flip_retraction();
    private:
        ctre::phoenix::motorcontrol::can::WPI_TalonSRX intake_motor{INTAKE_MOTOR};
        ctre::phoenix::motorcontrol::can::WPI_TalonFX actuation_motor{INTAKE_ACTUATION};
        bool retracted = true;
};
#endif