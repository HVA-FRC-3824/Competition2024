#ifndef INTAKE_H
#define INTAKE_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix6/TalonFX.hpp>
#include "rev/CANSparkMax.h"

class Intake : frc2::SubsystemBase
{
    public:
        Intake();
        void intake_actuate_simple(float input);
        void intake_actuate_point(float angle);
        void suck(float input);
        void robo_periodic();
        void flip_retraction();
        bool retracted = true;
    private:
        //ctre::phoenix::motorcontrol::can::WPI_TalonSRX intake_motor{INTAKE_MOTOR};
        ctre::phoenix6::hardware::TalonFX intake_angle_motor{INTAKE_ACTUATION,"rio"};

        rev::CANSparkMax intake_roller_motor{INTAKE_MOTOR, rev::CANSparkLowLevel::MotorType::kBrushless};
        //rev::CANSparkMax actuation_motor{INTAKE_ACTUATION, rev::CANSparkLowLevel::MotorType::kBrushless};

};
#endif