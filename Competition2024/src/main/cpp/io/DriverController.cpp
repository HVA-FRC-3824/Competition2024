#include "../../include/io/DriverController.hpp"

DriverController::DriverController(Swerve *swerve_obj)
{
    this->m_swerve = swerve_obj;
}

void DriverController::robo_periodic()
{
    if(DriverStick.GetRawButtonPressed(1)){m_swerve->gyro->Reset();}

    /* Right trigger */
    if(DriverStick.GetRawAxis(5) > .5)
    {
        this->m_swerve->move_speed = .5;
    } else {
        this->m_swerve->move_speed = SWERVE_DEFAULT_SPEED_MULTIPLIER;
    }  

    this->m_swerve->drive(-DriverStick.GetRawAxis(1),DriverStick.GetRawAxis(0),-DriverStick.GetRawAxis(4),this->m_swerve->gyro->GetYaw());
}