#include <iostream>
#include <unistd.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/io/OperatorController.hpp"

/// @brief Constructor for the OperatorController class.
/// @param intake - Pointer to the intake class to allow calling intake methods.
/// @param actuation - Pointer to the actuation class to allow calling actuation methods.
/// @param climb - Pointer to the climb class to allow calling climb methods.
OperatorController::OperatorController(Intake *intake, Actuation *actuation, Climb *climb)
{
    // Remember the method parameters
    m_intake    = intake;
    m_actuation = actuation;
    m_climb     = climb;
}

/// @brief Method called periodically every dirver/operator control packet.
void OperatorController::Robot_Periodic()
{
    std::cout << "OPERATOR\n";

    // Check for operator joystick button to ensure joystick is connected
    if (m_operator_joystick.GetRawButton(1))
       std::cout << "BUTTON!!\n";


    if (m_state)
    {
        frc::SmartDashboard::PutBoolean("Softlocked? ", false);

        // Return always after a hit condition to save resources
        if (m_state == O_SOFT_LOCK)
            return;
    
        // This is done below to prevent sticky values, since GetRawButtonPressed gets the value since it was last called
        // not firing the method since (ie. until a new mode is selected) will cause the old value to be used
        // better solution is to poll ALL buttons at once and then do actions based on the mode instead of vice versa

        double z  =  m_operator_joystick.GetZ();
        double y  = -m_operator_joystick.GetY();
        int index =  m_operator_joystick.GetRawAxis(3);

        if (z < .2 && z > -.2)   { z = 0; }
        if (y < .75 && y > -.75) { y = 0; } 

        m_actuation->Linear_Actuation(y);

        if (m_operator_joystick.GetPOV(0) == 0)
            m_intake->Intake_Actuate_Simple(-1);
        else if (m_operator_joystick.GetPOV(0) == 180)
            m_intake->Intake_Actuate_Simple(1);
        else 
            m_intake->Intake_Actuate_Simple(0);

        if (m_operator_joystick.GetRawButtonPressed(6))
            One_Button_Intake();

        // TOP_4
        if (m_operator_joystick.GetRawButtonPressed(4))
            this->m_actuation->Actuate_To_Point(0);

        // Climb control
        if (m_operator_joystick.GetPOV() == 0) 
        {
            m_climb->Climber_Control(CLIMBER_UP_POWER);
        }
        else if (m_operator_joystick.GetPOV() == 180)
        {
            m_climb->Climber_Control(CLIMBER_DOWN_POWER);
        }
    }
}

/// @brief Method to run the intake.
void OperatorController::One_Button_Intake()
{
    std::cout << "One_Button_Intake command start\n";

    // Ignore user input until command executes
    m_state = O_SOFT_LOCK; 

    // Transfer of note into index 
    m_intake->Intake_Actuate_Simple(.3);

    usleep(500 * 1000);
    
    // Return intake
    m_intake->Intake_Actuate_Simple(0);

    m_actuation->actuation_locked = false;
    m_state                       = O_ACTIVE; 
}

/// @brief Method to place in the amp.
void OperatorController::One_Button_Amp()
{
    m_state = O_SOFT_LOCK;

    usleep(500 * 1000);

    m_state = O_ACTIVE;
}
