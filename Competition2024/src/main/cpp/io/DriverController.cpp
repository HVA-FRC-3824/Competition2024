#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/io/DriverController.hpp"

 /// @brief Constructor for the DriverController class.
 /// @param swerve - Pointer to the swerve drive class.
DriverController::DriverController(Swerve *swerve)
{
    // Remember the swerve pointer
    this->m_swerve = swerve;
}

// @brief Method called periodically every driver/operator control packet.
void DriverController::Robot_Periodic()
{
    // Determine if the reset gyro joystick button was pressed
    if (m_driver_joystick.GetRawButtonPressed(1))
        m_swerve->navx.Reset();

    // Determine if the toggle X wheels button was pressed.
    if (m_driver_joystick.GetRawButtonPressed(2))
        m_swerve->Toggle_X_Wheels();

    // Determine if right bumper is pressed, makes it go faster/slower.
    if (m_driver_joystick.GetRawButtonPressed(6))
        m_swerve->Toggle_Fast_Wheels();
 
    // Get the joystick axis for the robot swerve drive control
    this->m_swerve->Drive(-m_driver_joystick.GetRawAxis(1), 
                           m_driver_joystick.GetRawAxis(0), 
                           m_driver_joystick.GetRawAxis(4), 
                           this->m_swerve->navx.GetYaw());
                           //this->m_swerve->navx.GetRoll() - m_swerve->m_gyro_offset);
}
