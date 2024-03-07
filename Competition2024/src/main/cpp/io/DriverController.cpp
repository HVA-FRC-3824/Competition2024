#include "../../include/io/DriverController.hpp"

DriverController::DriverController(Swerve *swerve_obj)
{
    this->m_swerve = swerve_obj;
}

void DriverController::robo_periodic()
{
    if(DriverStick.GetRawButtonPressed(1)){m_swerve->navx.Reset();}
    if(DriverStick.GetRawButtonPressed(6)){m_swerve->toggle_xwheels();}

    /* Right trigger */
    /*if(DriverStick.GetRawAxis(4) < -.5)
    {
        this->m_swerve->move_speed = .5;
    } else {
        this->m_swerve->move_speed = SWERVE_DEFAULT_SPEED_MULTIPLIER;
    }  */

    this->m_swerve->drive(DriverStick.GetRawAxis(1),-DriverStick.GetRawAxis(0),DriverStick.GetRawAxis(2),this->m_swerve->navx.GetYaw());
}