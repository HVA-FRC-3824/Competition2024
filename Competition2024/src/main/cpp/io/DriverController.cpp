#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/io/DriverController.hpp"

 /// @brief Constructor for the DriverController class.
 /// @param swerve - Pointer to the swerve drive class.
DriverController::DriverController(Swerve *swerve)
{
    // Remember the swerve pointer
    this->m_swerve = swerve;
    m_swerve->navx.Reset();
}

// @brief Method called periodically every driver/operator control packet.
void DriverController::Robot_Periodic()
{
    // Take Joystick inputs then correct/Sanitize them.
    // Graham wants his special stick controller >:(
    // float x  = -m_driver_joystick.GetRawAxis(1);
    // float y  = m_driver_joystick.GetRawAxis(0);
    // float x2 = m_driver_joystick.GetRawAxis(4);

    float x;
    float y;
    float x2;

    correct_inputs(&x,&y,&x2);

    // Determine if the reset gyro joystick button was pressed
    //if (m_driver_joystick.GetRawButtonPressed(1))
    //    m_swerve->navx.Reset();

    // Determine if the toggle X wheels button was pressed.
    if (m_driver_joystick.GetRawButtonPressed(2))
        m_swerve->Toggle_X_Wheels();

    // If the button is pressed down, do Toggle_Fast_Wheels().
    if (m_driver_joystick.GetRawButtonPressed(6))
        m_swerve->Toggle_Fast_Wheels();

 
    // Get the joystick axis for the robot swerve drive control
    this->m_swerve->Drive(x, 
                           y, 
                           x2, 
                           this->m_swerve->navx.GetYaw());
                           //this->m_swerve->navx.GetRoll() - m_swerve->m_gyro_offset);
}

/// @brief Method to create dead zones for the controller joysticks and set exponential inputs for finer control.
/// @param x - Pointer to the x stick value to return the value used.
/// @param y - Pointer to the y stick value to return the value used.
/// @param x2 - Pointer to the second x stick value to return the value used.
void DriverController::correct_inputs(float *x, float *y, float *x2)
{
    // Pass in the values and they are corrected
    bool y_deadzone = false;
    bool x_deadzone = false;
    bool y_move_abs = false;

    // Ignore our deadzone and fix the moving forward issue
    if (*y < DEADZONE_THRESHOLD && *y > -DEADZONE_THRESHOLD)
    {
        *y = 0;
        y_deadzone = true;
    }   

    if (*x < DEADZONE_THRESHOLD && *x > -DEADZONE_THRESHOLD)
    {
        *x = 0;
        x_deadzone = true;
        if (!y_deadzone)
        {
            // Sets so that the true forward value is used incase of no strafing
            y_move_abs = true;
        }
    } 

    if (*x2 < DEADZONE_THRESHOLD && *x2 > -DEADZONE_THRESHOLD)
    {
        *x2 = 0;
    }

    y_deadzone = false;
    x_deadzone = false;
    y_move_abs = false;

    // ### exponential input ###

    // same thing for all three

    // Ignore joystick input if it's too small
    if (abs(*x) > DEADZONE_THRESHOLD){
        // Normalize to 128 (most controllers are -127 to 127
        *x *= 128;
        // Direction is either 1 or -1, based on joystick value
        int direction = abs(*x) / *x;
        // Plug joystick value into exponential function
        *x = direction * (1.2 * pow(1.0356, abs(*x)) - 1.2 + 0.2 * abs(*x));}
    // Normalize back to -1 to 1
    *x /= 128;

    if (abs(*y) > DEADZONE_THRESHOLD){
        // Normalize to 128 (most controllers are -127 to 127
        *y *= 128;
        // Direction is either 1 or -1, based on joystick value
        int direction = abs(*y) / *y;
        // Plug joystick value into exponential function
        *y = direction * (1.2 * pow(1.0356, abs(*y)) - 1.2 + 0.2 * abs(*y));}
    // Normalize back to -1 to 1
    *y /= 128;

    if (abs(*x2) > DEADZONE_THRESHOLD){
        // Normalize to 128 (most controllers are -127 to 127
        *x2 *= 128;
        // Direction is either 1 or -1, based on joystick value
        int direction = abs(*x2) / *x2;
        // Plug joystick value into exponential function
        *x2 = direction * (1.2 * pow(1.0356, abs(*x)) - 1.2 + 0.2 * abs(*x2));}
    // Normalize back to -1 to 1
    *x2 /= 128;
}