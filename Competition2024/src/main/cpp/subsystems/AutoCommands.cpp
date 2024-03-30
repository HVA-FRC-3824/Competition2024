#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/subsystems/AutoCommands.hpp"

using namespace frc;

/// @brief Constructor for the AutoCommands class.
/// @param swerve - Pointer to the robot swerve subsystem.
/// @param intake - Pointer to the robot intake subsystem.
AutoCommands::AutoCommands(Swerve *swerve, Intake *intake)
{
    // Remember the swerve and intake subsystem pointers
    m_swerve = swerve;
    m_intake = intake;
}

/// @brief Autonomous command to do nothing.
void AutoCommands::Auto_Do_Nothing()
{
    SmartDashboard::PutString("AutoName: ", "Do Nothing");
}

/// @brief Autonomous to drive forward.
void AutoCommands::Auto_Move()
{
    // Drive forward
    m_swerve->Drive(AUTO_DRIVE_FORWARD_SPEED, 0, 0, 0);
    usleep(AUTO_DRIVE_FORWARD_TIME);   // Timed drive
    m_swerve->Drive(0, 0, 0, 0);       // Stop
}

/// @brief  Autonomous comand to place a note in the Amp, and move out of the tape.
/// @param move_after_amp - Bool to indacate if it should move after AMP.
void AutoCommands::Auto_Place_Amp_And_Move(bool red_alliance)
{
    double drive_direction = 0;
    if (red_alliance) 
        drive_direction =  1;
    else
        drive_direction = -1;

    // Set intake angle to amp
    m_intake->MoveToAmp();

    // Move left in front of amp
    m_swerve->Drive(0, drive_direction * AUTO_SCOOT_TO_AMP_SPEED, 0, 0);
    usleep(AUTO_SCOOT_TO_AMP_TIME);       // Timed drive
    m_swerve->Drive(0, 0, 0, 0);          // Stop

    // Move forward in front of amp
    m_swerve->Drive(AUTO_DRIVE_TO_AMP_SPEED, 0, 0, 0); 
    usleep(AUTO_DRIVE_TO_AMP_TIME);       // Timed drive
    m_swerve->Drive(0, 0, 0, 0);          // Stop
    
    // Place note in amp
    m_intake->Set_Roller_Motors(AUTO_PLACE_NOTE);
    usleep(AUTO_PLACE_NOTE_TIME);         // Timed to Place it
    m_intake->Set_Roller_Motors(0);       // Stop Roller Motors

    // Back up to remove intake from AMP
    m_swerve->Drive(-AUTO_DRIVE_TO_AMP_SPEED, 0, 0, 0); 
    usleep(AUTO_DRIVE_TO_AMP_TIME);       // Timed drive
    m_swerve->Drive(0, 0, 0, 0);          // Stop

    // Move across line
    m_swerve->Drive(0, drive_direction * AUTO_DRIVE_ACROSS_LINE_SPEED, 0, 0);
    usleep(AUTO_DRIVE_ACROSS_LINE_TIME);  // Timed drive
    m_swerve->Drive(0, 0, 0, 0);          // Stop
}
