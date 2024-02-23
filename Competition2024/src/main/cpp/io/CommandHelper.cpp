/* secretely C not C++... :D */
#include "../../include/io/CommandHelper.hpp"

#include "../../include/io/OperatorController.hpp"

OperatorController *o_controller;
cmd_share *control;

/* Handles command logic customly because WPILib is for nerds 
   loop runs every async of periodic because why not they're on different threads anyways
   loop checks for a flag to run and then runs the preprogrammed function from OperatorController
   up to command maker to place in proper checks and balances :D
*/

void *command_thread(void *parm)
{
    control->state = C_ACTIVE;
    control->my_wishes = C_NONE;

    switch(control->command_being_run)
    {
        case C_INTAKE_OB:
            o_controller->one_button_intake();
            break;
        case C_NONE:
            break;
    }

    control->state = C_INACTIVE;
    control->command_being_run = C_NONE;
}

void *command_runner(void *parm)
{
    pthread_t command_ref;
    while(1)
    {
        /* Terminatable, resets to old state */
        if(control->state == C_KILL)
        {
            pthread_cancel(command_ref);
            control->state = C_INACTIVE;
            control->my_wishes = C_NONE;
        }

        if(control->state == C_DISABLED){continue;}

        /* Run logic */
        if(control->my_wishes == C_RUN && control->state == C_INACTIVE)
        {
            pthread_create(&command_ref,NULL,command_thread,NULL);
        }
    }
}



void runner_launcher(cmd_share *control_in, OperatorController *controller)
{
    control = control_in;
    o_controller = controller;
    pthread_t thread;
    pthread_create(&thread, NULL, command_runner, NULL);
}