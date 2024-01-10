#include "../../include/io/OperatorController.hpp"
#include <iostream>

void OperatorController::test_command()
{
    std::cout << "RAAAH";
}

OperatorController::OperatorController(Turret *turret_obj)
{
    std::cout << "asdas";
    frc2::JoystickButton TestButton{&OperatorStick,1};
    frc2::JoystickButton TurretTestButton{&OperatorStick,2};
    TurretTestButton.OnTrue(&turret_obj->turret_test_command);
    TestButton.OnTrue(&GODDAMNIT);
}