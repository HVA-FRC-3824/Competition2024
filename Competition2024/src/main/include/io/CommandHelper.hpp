#include "../Constants.h"
#include "OperatorController.hpp"
#include <stdint.h>
#include <pthread.h>

#ifndef COMMAND_HELPER_H
#define COMMAND_HELPER_H

void *command_thread(void *parm);
void *command_runner(void *parm);
void runner_launcher(cmd_share *control_in, OperatorController *controller);

#endif