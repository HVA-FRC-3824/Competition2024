#include "../../include/subsystems/Launcher.hpp"
#include <iostream>

Launcher::Launcher()
{
    this->LeftSpin.SetNeutralMode(ctre::phoenix::motorcontrol::Brake);
    this->RightSpin.SetNeutralMode(ctre::phoenix::motorcontrol::Brake);
    this->Index.SetNeutralMode(ctre::phoenix::motorcontrol::Brake);
    std::cout << "asdfasdf\n";
}

/* This is our periodic function, it is apart of every subsytems, runs every time the robot cycles (every 20 ms)*/
void Launcher::Periodic()
{
    /* Update current_velocity by reading it from the TalonFX's, remember to have one motor reversed! */
    std::cout << "Hello\n";
}

void Launcher::hold_velocity(float velocity)
{
}

void Launcher::simple_spin(float input)
{
    /* Spin based on input */
    /* https://store.ctr-electronics.com/content/api/cpp/html/classctre_1_1phoenix_1_1motorcontrol_1_1can_1_1_w_p_i___talon_f_x.html */
    /* Your motors are defined in Launcher.hpp*/
    /*ctre::phoenix6::controls::DutyCycleOut LEFT_REQ{0.0};
    ctre::phoenix6::controls::DutyCycleOut RIGHT_REQ{0.0};
    LeftSpin.SetControl(LEFT_REQ.WithOutput(input));
    RightSpin.SetControl(RIGHT_REQ.WithOutput(-input));  */
    LeftSpin.Set(input);
    RightSpin.Set(-input);
}

void Launcher::index_spin(float input)
{
    Index.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, input);
}