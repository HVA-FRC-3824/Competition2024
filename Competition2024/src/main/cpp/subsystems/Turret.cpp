#include "../../include/subsystems/Turret.hpp"
#include <iostream>

Turret::Turret(angle_mem_share *share)
{
    this->TURRET_MOTOR.Config_kP(0,TURRET_P);
    this->TURRET_MOTOR.Config_kI(0,TURRET_I);
    this->TURRET_MOTOR.Config_kD(0,TURRET_D);
    this->TURRET_MOTOR.ConfigIntegratedSensorAbsoluteRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);
    internal_reference = share;
}

void Turret::robo_periodic()
{
    /* Update live data to operator view */
    frc::SmartDashboard::PutBoolean("Turret Locked? ", this->locked);
    frc::SmartDashboard::PutNumber("Turret Heading: ", this->current_heading);

    /* Read values from encoder to find accurate heading, and assign it to current_heading */
    this->current_heading = this->TURRET_MOTOR.GetSelectedSensorPosition(0)/TURRET_ROTATIONS_PER_360;

    /* This stops the turret from internally going above 360, preventing snapback issues! */
    /*if(this->current_heading >= 360)
    {
        this->current_heading = this->current_heading - 360;
        this->TURRET_MOTOR.Set(ctre::phoenix::motorcontrol::ControlMode::Position,((this->current_heading/360)*TURRET_ROTATIONS_PER_360));
    }
    this->internal_reference->turret_heading = this->current_heading; */
}

void Turret::spin_to_angle(int angle)
{
    if(!locked)
    {
        this->TURRET_MOTOR.Set(ctre::phoenix::motorcontrol::ControlMode::Position,((angle/360)*TURRET_ROTATIONS_PER_360));
    }
} 

/* More like snap to heading of chassis, warning: may be useless (just snap to zero) */
// Worthless function, snap_to_axis(0); works similiarly
// NOTE: keeping this incase it works better in the future? I don't really know
void Turret::snap_to_swerve()
{
    if(!locked)
    {
        /* Set target angle to swerves */
        spin_to_angle(this->internal_reference->swerve_heading);
    }
}  

void Turret::snap_to_axis(int heading)
{
    if(!locked)
    {
        /* I know this is counterintuitive but motor will be zeroed once the turret is faced forwards it'll be zero */
        int desired = 0;
        switch(heading)
        {
            case 0: desired = 0; break;   /* Front */
            case 1: desired = 90; break;  /* Left  */
            case 2: desired = 180; break; /* Back  */
            case 3: desired = 270; break; /* Right */
            default: desired = 0;
        }
        spin_to_angle(desired);
        std::cout << desired << "\n";
        std::cout << this->TURRET_MOTOR.GetSelectedSensorPosition() << "\n";
    }
}

void Turret::lock_turret()
{
    this->locked = !this->locked;
}    

void Turret::spin_simple(float percent)
{
    if(!locked)
    {
       this->TURRET_MOTOR.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,percent); 
    }
}

void Turret::turret_test()
{
    if(test_heading == 3)
    {
        snap_to_axis(test_heading);
        test_heading = 0;
        return;
    }
    snap_to_axis(test_heading);
    test_heading++;
}