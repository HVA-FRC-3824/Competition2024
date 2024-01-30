#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
using namespace ctre::phoenix::motorcontrol::can;

class Launcher : frc2::SubsystemBase
{
    public:
        float current_velocity;
        Launcher();
        void Periodic() override;
        void simple_spin(float input);      /* Spin based on simple input*/
        void hold_velocity(float velocity); /* Spin and hold velocity    */
    private:
        WPI_TalonFX LeftSpin{LEFT_MOTOR_CAN};
        WPI_TalonFX RightSpin{RIGHT_MOTOR_CAN};
};


#endif