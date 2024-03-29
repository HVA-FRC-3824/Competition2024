#include <iostream>

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
///
/// Operator:
///    Button POV_0:   Intake move to Feed.
///    Button POV_180: Intake move to amp.
///
///    Button A:       Step intake negative.
///    Button Y:       Step intake positive.
///    Button X:       Flib intake to opposite position.
///
///    Button Bumper   Right: Climb retract (Climb).
///    Button Bumper   Right: Climb extend.
///
///    Button POV_90:  Enable cliber motors.
///    Button POV_270: Disable cliber motors.
void OperatorController::Robot_Periodic()
{
    // Intake rollers
    double y  = -m_operator_joystick.GetY();
    if (y < INTAKE_ROLLER_MOTOR_JOYSTICK_DEADBAND && y > -INTAKE_ROLLER_MOTOR_JOYSTICK_DEADBAND)
        y = 0; 
    m_intake->Set_Roller_Motors(y);

    // Extend/retract the intake
    if (m_operator_joystick.GetPOV() == JOYSTICK_POV_0)
        m_intake->MoveToFeed();
    else if (m_operator_joystick.GetPOV() == JOYSTICK_POV_180)
        m_intake->MoveToAmp();

    // Flip the intake
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_X))
        m_intake->Flip_Retraction();

    // Check for intake angle lower offset (in case belt slips)
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_A))
       m_intake->AddIntakeOffset(-INTAKE_POSITION_STEP_VALUE);
        
    // Check for intake angle raise offset (in case belt slips)
    else if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_Y))
        m_intake->AddIntakeOffset(INTAKE_POSITION_STEP_VALUE);
        
    // Climb control extend
    //if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUMPER_LEFT))
    //    m_climb->Extend();

    // Climb control retract
    //if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUMPER_RIGHT))
    //    m_climb->Retract();

    // Climb control 
    if (m_operator_joystick.GetRawButton(JOYSTICK_BUMPER_LEFT))
        m_climb->Set_Motor_Output(CLIMBER_UP_POWER);  // Extend
    else if (m_operator_joystick.GetRawButton(JOYSTICK_BUMPER_RIGHT))
       m_climb->Set_Motor_Output(CLIMBER_DOWN_POWER);  // Retract
    else
        m_climb->Set_Motor_Output(0.0);  // Stop
 
    // Enable the climber motors
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_POV_90))
        m_climb->Enable_Climber_Motors(true);

    // Disable the climber motors
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_POV_270))
        m_climb->Enable_Climber_Motors(false);
}
