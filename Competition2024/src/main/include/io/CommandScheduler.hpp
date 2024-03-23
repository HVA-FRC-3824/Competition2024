#include <stdint.h>
#include <pthread.h>
#include "OperatorController.hpp"
#include "../Constants.hpp"
#include "../subsystems/AutoCommands.hpp"

/// @brief Method to configure the command scheduler.
/// @param commandParameterss - Pointer to the command, command state and desired action parameters.
/// @param controller - Pointer to the operator controller class to allow calling operator controller methods.
void Command_Configure(CommandParameters *commandParameterss, OperatorController *controller);

/// @brief Method called periodically to determine the command action.
void Command_Runner();

/// @brief The command scheduler thread to run the command.
/// @param parmeter - Thread parameter required for creating the thread (not used).
/// @return Command operational state.
void *Command_Thread(void *parmeter);
