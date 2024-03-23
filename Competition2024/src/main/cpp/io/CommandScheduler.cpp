#include <iostream>
#include "../../include/io/CommandScheduler.hpp"
#include "../../include/io/OperatorController.hpp"
#include "../../include/subsystems/AutoCommands.hpp"

// Define the local variables
OperatorController *m_controller;
CommandParameters  *m_command_parameters;
pthread_t           m_command_parameters_Thread;

// Handles command logic.
// Loop runs every async of periodic because why not they're on different threads anyways
// loop checks for a flag to run and then runs the preprogrammed function from OperatorController
// up to command maker to place in proper checks and balances

/// @brief Method to configure the command scheduler.
/// @param commandParameterss - Pointer to the command, command state and desired action parameters.
/// @param controller - Pointer to the operator controller class to allow calling operator controller methods.
void Command_Configure(CommandParameters *commandParameters, OperatorController *controller)
{
    // Remember the command parameters
    m_command_parameters = commandParameters;
    m_controller         = controller;
}

/// @brief Method called periodically to determine the command action.
void Command_Runner()
{
    // Command run logic to start the command
    if (m_command_parameters->action == C_RUN && m_command_parameters->state == C_INACTIVE)
    {
        std::cout << "Command logic ran\n";

        // Indicate that the thread has been started
        m_command_parameters->action = C_NONE;
        m_command_parameters->state  = C_ACTIVE;

        // Create the command thread passing the command
        pthread_create(&m_command_parameters_Thread, NULL, Command_Thread, NULL);
    }

    // Terminatable, resets to old state 
    if (m_command_parameters->action == C_KILL && m_command_parameters->state == C_ACTIVE)
    {
        // Indicate that the thread has been cancelled
        m_command_parameters->action = C_NONE;
        m_command_parameters->state  = C_INACTIVE;

        // Cancel the command thread
        pthread_cancel(m_command_parameters_Thread);
    }
}

/// @brief The command scheduler thread to run the command.
/// @param parmeter - Thread parameter required for creating the thread (not used).
/// @return Command operational state.
void *Command_Thread(void *parmeter)
{
    // Determine the command to run
    switch (m_command_parameters->command)
    {
        case C_INTAKE_OB:
        {
            // Run the one button intake command
            m_controller->One_Button_Intake();
            break;
        }

        case C_AMP_OB:
        {
            // Run the one button load amp
            m_controller->One_Button_Amp();
            break;
        }
    }

    // Indicate that the command was completed            
    m_command_parameters->command = C_KILL;
    return 0;
}