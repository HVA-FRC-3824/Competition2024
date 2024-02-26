#include "../../include/subsystems/Actuation.hpp"
#include <iostream>

Actuation::Actuation()
{
    this->actuation_motor.Config_kP(0,ACTUATION_P);
    this->actuation_motor.Config_kI(0,ACTUATION_I);
    this->actuation_motor.Config_kD(0,ACTUATION_D);
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    this->actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor,0,0);
    this->actuation_motor.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
    this->actuation_motor.ConfigPeakOutputForward(.2);
    this->actuation_motor.ConfigPeakOutputReverse(-.2);
}

void Actuation::actuate_to_point(float point)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position,(point/360)*ACTUATION_UFR);
    }
}

void Actuation::linear_actuation(float input)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
    }
}

void Actuation::robo_periodic()
{
    std::cout << "ACT_A: " << (this->actuation_motor.GetSelectedSensorPosition()/ACTUATION_UFR)*360 << "\n";
    //std::cout << "ACT_A: " << this->actuation_motor.GetSelectedSensorPosition() << "\n";
}