#include <iostream>
#include <unistd.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/io/OperatorController.hpp"

/// @brief Constructor for the OperatorController class.
/// @param intake - Pointer to the intake class to allow calling intake methods.
/// @param climb - Pointer to the climb class to allow calling climb methods.
OperatorController::OperatorController(Intake *intake, Climb *climb)
{
    // Remember the method parameters
    m_intake = intake;
    m_climb  = climb;
}

/// @brief Method called periodically every dirver/operator control packet.
void OperatorController::Robot_Periodic()
{
    // Intake rollers
    double y  = -m_operator_joystick.GetY();
    if (y < INTAKE_MOTOR_JOYSTICK_DEADBAND && y > -INTAKE_MOTOR_JOYSTICK_DEADBAND)
        y = 0; 
    m_intake->Set_Roller_Motors(y);

    // Extend/retract the intake
    if (m_operator_joystick.GetPOV() == JOYSTICK_POV_0)
        m_intake->Extend();
    else if (m_operator_joystick.GetPOV() == JOYSTICK_POV_180)
        m_intake->Retract();

    // Flip the intake
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_A))
        m_intake->Flip_Retraction();

    // Climb control
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_X))
        m_climb->Extend();
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_Y))
        m_climb->Retract();
}
