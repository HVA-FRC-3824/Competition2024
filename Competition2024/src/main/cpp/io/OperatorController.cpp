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
    frc::SmartDashboard::PutBoolean("Softlocked? ", false);

    // Intake rollers
    double y  = -m_operator_joystick.GetY();
    if (y < 0.75 && y > -0.75)
        y = 0; 
    m_intake->Set_Roller_Motors(y);

    // Extend/retract the intake
    if (m_operator_joystick.GetPOV(0) == 0)
        m_intake->Extend_Intake();
    else if (m_operator_joystick.GetPOV(0) == 180)
        m_intake->Retract_Intake();

    // Flip the intake
    if (m_operator_joystick.GetRawButtonPressed(1))
        m_intake->Flip_Retraction();

    // Climb control
    if (m_operator_joystick.GetRawButtonPressed(3))
        m_climb->Climber_Control(CLIMBER_UP_POWER);
    else if (m_operator_joystick.GetRawButtonPressed(4))
        m_climb->Climber_Control(CLIMBER_DOWN_POWER);
}
