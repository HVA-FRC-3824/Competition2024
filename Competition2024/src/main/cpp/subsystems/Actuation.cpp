#include "../../include/subsystems/Actuation.hpp"

Actuation::Actuation()
{
    this->actuation_motor.Config_kP(0,ACTUATION_P);
    this->actuation_motor.Config_kI(0,ACTUATION_I);
    this->actuation_motor.Config_kD(0,ACTUATION_D);
    this->actuation_motor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    this->actuation_motor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor,0,0);
    this->actuation_motor.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
    this->actuation_motor.ConfigPeakOutputForward(.5);
    this->actuation_motor.ConfigPeakOutputReverse(.5);
}

void Actuation::actuate_to_point(float point)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::Position,(point/360)*TALONFX_UFR);
    }
}

void Actuation::linear_actuation(float input)
{
    if(!locked)
    {
        this->actuation_motor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
    }
}

void Actuation::Periodic()
{
    /* Smartdashboard locked status */
}