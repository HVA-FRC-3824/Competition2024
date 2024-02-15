#include "../../include/subsystems/Launcher.hpp"
#include <iostream>

Launcher::Launcher()
{
    this->LeftSpin.Config_kP(0,LAUNCH_MOTOR_P);
    this->LeftSpin.Config_kI(0,LAUNCH_MOTOR_I);
    this->LeftSpin.Config_kD(0,LAUNCH_MOTOR_D);

    this->RightSpin.Config_kP(0,LAUNCH_MOTOR_P);
    this->RightSpin.Config_kI(0,LAUNCH_MOTOR_I);
    this->RightSpin.Config_kD(0,LAUNCH_MOTOR_D);

    std::cout << "asdfasdf\n";
}

/* This is our periodic function, it is apart of every subsytems, runs every time the robot cycles (every 20 ms)*/
void Launcher::Periodic()
{
    /* Update current_velocity by reading it from the TalonFX's, remember to have one motor reversed! */
    std::cout << "Hello\n";
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

void Launcher::index_spin(float input)
{
    Index.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, input);
}