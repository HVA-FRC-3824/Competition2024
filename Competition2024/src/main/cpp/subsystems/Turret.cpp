#include "../../include/subsystems/Turret.hpp"

Turret::Turret()
{
    this->TURRET_MOTOR.Config_kP(.5,TURRET_P);
    this->TURRET_MOTOR.Config_kI(0,TURRET_I);
    this->TURRET_MOTOR.Config_kD(0,TURRET_D);
    this->TURRET_MOTOR.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
}

void Turret::Periodic()
{
    /* Update live data to operator view */
    frc::SmartDashboard::PutBoolean("Turret Locked: ", this->locked);
    frc::SmartDashboard::PutNumber("Turret Heading: ", this->current_heading);

    /* Read values from encoder to find accurate heading, and assign it to current_heading */
    this->current_heading = this->TURRET_MOTOR.GetSelectedSensorPosition(0)/TURRET_ROTATIONS_PER_360;
}

void Turret::spin_to_angle(int angle)
{
    if(~locked)
    {
        /* Work on a non snapback thingy */
        if(angle < 0 && current_heading >= 0)
        {

        }
        this->TURRET_MOTOR.Set(ctre::phoenix::motorcontrol::ControlMode::Position,((angle/360)*TURRET_ROTATIONS_PER_360));
    }
} 

void Turret::snap_to_swerve()
{
    if(~locked)
    {
        /* TODO: fix your mfing memory buddy */
    }
}  

void Turret::snap_to_axis(int heading)
{
    if(~locked)
    {
        int desired = 0;
        switch(heading)
        {
            case 0: desired = 0;   /* Front */
            case 1: desired = 90;  /* Right */
            case 2: desired = -90; /*  Left */
            case 3: desired = 180; /*  Back */
            default: desired = 0;
        }
        spin_to_angle(desired);
    }
}

void Turret::lock_turret()
{
    this->locked = ~this->locked;
}    

void Turret::spin_simple(float percent)
{
    if(~locked)
    {
       this->TURRET_MOTOR.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,percent); 
    }
}