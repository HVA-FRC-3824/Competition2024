#include "../../include/io/DriverController.hpp"

DriverController::DriverController(Swerve *swerve_obj)
{
    this->m_swerve = swerve_obj;
}

void DriverController::robo_periodic()
{
    if(DriverStick.GetRawButtonPressed(1)){this->m_swerve->toggle_field_centricity();}

    this->m_swerve->drive(-DriverStick.GetRawAxis(1),DriverStick.GetRawAxis(0),-DriverStick.GetRawAxis(2),this->m_swerve->gyro->GetYaw());
}