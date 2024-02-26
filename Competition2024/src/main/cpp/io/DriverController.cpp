#include "../../include/io/DriverController.hpp"

DriverController::DriverController(Swerve *swerve_obj)
{
    this->m_swerve = swerve_obj;
}

void DriverController::robo_periodic()
{
    if(DriverStick.GetRawButtonPressed(1)){this->m_swerve->toggle_field_centricity();}

    this->m_swerve->drive(-DriverStick.GetY(),DriverStick.GetX(),-DriverStick.GetRawAxis(4),this->m_swerve->gyro->GetYaw());
}