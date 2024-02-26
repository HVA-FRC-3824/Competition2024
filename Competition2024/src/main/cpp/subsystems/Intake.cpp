#include "../../include/subsystems/Intake.hpp"
#include <iostream>

Intake::Intake()
{
    this->actuation_motor.Config_kP(0,INTAKE_ACTUATION_P);
    this->actuation_motor.Config_kI(0,INTAKE_ACTUATION_I);
    this->actuation_motor.Config_kD(0,INTAKE_ACTUATION_D);
    this->actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor,0,0);
    this->actuation_motor.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    this->actuation_motor.ConfigPeakOutputForward(.2);
    this->actuation_motor.ConfigPeakOutputReverse(-.18);
}

void Intake::intake_actuate_simple(float input)
{
    this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, input);
}

void Intake::intake_actuate_point(float angle)
{
    //this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, (angle/360)*INTAKE_ACTUATION_UFR);
    this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position, angle);
}

void Intake::suck(float input)
{
    this->intake_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
};

void Intake::robo_periodic()
{
    std::cout << "INTAKE POS: " << this->actuation_motor.GetSelectedSensorPosition(0) << "\n";
}


/* This works with the correct position for it being at 0 */
void Intake::flip_retraction()
{
    if(retracted)
    {
        std::cout << "non retracted\n";
        retracted = false;
        intake_actuate_point(INTAKE_BOTTOM_POINT);
    } else 
    {
        std::cout << "retracted\n";
        retracted = true;
        intake_actuate_point(0);
    }
}