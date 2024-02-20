#include "../../include/subsystems/Intake.hpp"

Intake::Intake()
{
    this->actuation_motor.Config_kP(0,INTAKE_ACTUATION_P);
    this->actuation_motor.Config_kI(0,INTAKE_ACTUATION_I);
    this->actuation_motor.Config_kD(0,INTAKE_ACTUATION_D);
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}

void Intake::intake_actuate_simple(float input)
{
    this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, input);
}

void Intake::intake_actuate_point(float angle)
{
    this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, angle);
}

void Intake::suck(float input)
{
    this->intake_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
};