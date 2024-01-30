#include "../../include/io/OperatorController.hpp"
#include <iostream>

void OperatorController::test_command()
{
    this->m_turret->snap_to_axis(3);
}

void OperatorController::reset_gyro()
{
    this->ahrs->Reset();
}

OperatorController::OperatorController(Turret *turret_obj, AHRS *ahrs_obj, Launcher *launcher_obj)
{
    m_turret = turret_obj;
    ahrs = ahrs_obj;
    m_launcher = launcher_obj;

    std::cout << "asdas";
    frc2::JoystickButton TestButton{&OperatorStick,1};
    frc2::JoystickButton TurretTestButton{&OperatorStick,2};
    frc2::JoystickButton ResetGyro{&OperatorStick,3};
    
    TurretTestButton.OnTrue(&turret_obj->turret_test_command);
    TestButton.OnTrue(&test_command2);
    ResetGyro.OnTrue(&reset_gyro_command);
}