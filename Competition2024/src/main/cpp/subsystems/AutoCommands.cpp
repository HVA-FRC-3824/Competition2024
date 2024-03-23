#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/AutoCommands.hpp"
#include "../../include/subsystems/Actuation.hpp"

// Note on commands in general: each command runs in a thread independant of this current thread
//so if you NEED to wait for a command to complete then you must wait in this thread

// IMPORTANT! Make sure ALL sleeps and delays do not total over 15 seconds for one auto

/// @brief Constructor for the AutoCommands class.
/// @param swerve - Pointer to the robot swerve subsystem.
/// @param actuation - Pointer to the robot actuation subsystem.
AutoCommands::AutoCommands(Swerve *swerve, Actuation *actuation)
{
    // Remember the swerve and actuation subsystem pointers
    m_swerve    = swerve;
    m_actuation = actuation;
}

/// @brief Autonomous command to do nothing.
void AutoCommands::Auto_Do_Nothing()
{
    frc::SmartDashboard::PutString("AutoName: ", "Do Nothing");
}

/// @brief Autonomous to drive forward.
void AutoCommands::Auto_Move()
{
    // Drive forward
    m_swerve->Drive(-.4, 0, 0, 0);

    // Drive for two seconds
    usleep(2000 * 1000);

    // Stop
    m_swerve->Drive(0, 0, 0, 0);
}
