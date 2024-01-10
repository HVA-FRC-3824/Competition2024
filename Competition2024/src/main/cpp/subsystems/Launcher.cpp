#include "../../include/subsystems/Laucher.hpp"

/* The launcher works by spinning two wheels at the same time in opposite directions
    the wheels are controlled by two motors, which are controlled by TalonFX's
    The goal here is to make two functions, one that spins based on a controller input (given as a parameter),
    the other to hold a certain velocity (see controlmodes for TalonFX's! )


    For some code examples of Talons you can also look at Turret.cpp (hint for the percent out or controller input one
    the code is already there!)

    I've left some template code for the first function, the second you must code from scratch

    All definitions are in Launcher.hpp

    Bonus: setup each motor to be used with PIDs in the constructor (see Turret for example, you'll need to create constants in Constants.h)
 */

Launcher::Launcher()
{
    /* Constructor, will have code later, IGNORE */
}

/* This is our periodic function, it is apart of every subsytems, runs every time the robot cycles (every 20 ms)*/
void Launcher::Periodic()
{
    /* Update current_velocity by reading it from the TalonFX's, remember to have one motor reversed! */
}

void Launcher::simple_spin(float input)
{
    /* Spin based on input */
    /* https://store.ctr-electronics.com/content/api/cpp/html/classctre_1_1phoenix_1_1motorcontrol_1_1can_1_1_w_p_i___talon_f_x.html */
    /* Your motors are defined in Launcher.hpp*/
}