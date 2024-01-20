#include "../../include/subsystems/Laucher.hpp"

/* The launcher works by spinning two wheels at the same time in opposite directions
    the wheels are controlled by two motors, which are controlled by TalonFX's
    The goal here is to make two functions, one that spins based on a controller input (given as a parameter),
    the other to hold a certain velocity (see controlmodes for TalonFX's! )

    The end goal is to have adjustable and predefined velocities for the launcher (which requires some testing and PIDs)

    For some code examples of Talons you can also look at Turret.cpp (hint for the percent out or controller input one
    the code is already there!)

    I've left some template code for the first function, the second you must code from scratch

    All definitions are in Launcher.hpp

    Bonus: setup each motor to be used with PIDs in the constructor (see Turret for example, you'll need to create constants in Constants.h)
 */

Launcher::Launcher()
{
    /* Constructor*/
    //sets PID values for motors
    this->LeftSpin.Config_kP(.5,LAUNCH_MOTOR_P);
    this->LeftSpin.Config_kI(0,LAUNCH_MOTOR_I);
    this->LeftSpin.Config_kD(0,LAUNCH_MOTOR_D);

    this->RightSpin.Config_kP(.5,LAUNCH_MOTOR_P);
    this->RightSpin.Config_kI(0,LAUNCH_MOTOR_I);
    this->RightSpin.Config_kD(0,LAUNCH_MOTOR_D);
}

/* This is our periodic function, it is apart of every subsytems, runs every time the robot cycles (every 20 ms)*/
void Launcher::Periodic()
{
    /* Update current_velocity by reading it from the TalonFX's, remember to have one motor reversed! */
}

void Launcher::hold_velocity(float velocity) {
    /*basically copied from simple_spin*/
    /* Your motors are defined in Launcher.hpp*/
    LeftSpin.Set(ctre::phoenix::motorcontrol::ControlMode::Velocity,velocity);
    RightSpin.Set(ctre::phoenix::motorcontrol::ControlMode::Velocity, -velocity);
}

void Launcher::simple_spin(float input)
{
    /* Spin based on input */
    /* https://store.ctr-electronics.com/content/api/cpp/html/classctre_1_1phoenix_1_1motorcontrol_1_1can_1_1_w_p_i___talon_f_x.html */
    /* Your motors are defined in Launcher.hpp*/
    LeftSpin.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,input);
    RightSpin.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -input);
}
