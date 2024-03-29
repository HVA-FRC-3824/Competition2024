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

    frc::SmartDashboard::PutNumber("periodic_counter: ", periodic_counter);

    // Check for intake angle lower offset (in case belt slips)
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_A))
    {
        // Determine if the button is being debounced
        // (i.e., the button will not be processed if it's debounce count is greater than the present decounce_counter)
        if (intake_lower_debounce_count < periodic_counter)
        {
           // Remember when the button was pressed
           intake_lower_debounce_count = periodic_counter + BUTTON_DEBOUNCE_COUNTS;

           frc::SmartDashboard::PutNumber("Button A: ", intake_lower_debounce_count);

           // Precess the button press
           m_intake->AddIntakeOffset(-INTAKE_POSITION_STEP_VALUE);
        }
    }

    // Check for intake angle raise offset (in case belt slips)
    else if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUTTON_Y))
    {
        // Determine if the button is being debounced
        // (i.e., the button will not be processed if it's debounce count is greater than the present decounce_counter)
        if (intake_raise_debounce_count < periodic_counter)
        {
           // Remember when the button was pressed
           intake_raise_debounce_count = periodic_counter + BUTTON_DEBOUNCE_COUNTS;

           frc::SmartDashboard::PutNumber("Button Y: ", intake_raise_debounce_count);

           // Precess the button press
           m_intake->AddIntakeOffset(INTAKE_POSITION_STEP_VALUE);
        }
    }

    // Climb control extend
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUMPER_LEFT))
        m_climb->Extend();

    // Climb control retract
    if (m_operator_joystick.GetRawButtonPressed(JOYSTICK_BUMPER_RIGHT))
        m_climb->Retract();

    // increment the debounce couner
    periodic_counter++;
}
