#ifndef OPERATOR_CONTROLLER_H
#define OPERATOR_CONTROLLER_H
#include "../Constants.h"
#include "../subsystems/Turret.hpp"
#include <frc2/command/SubsystemBase.h>
#include <frc/Joystick.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/PrintCommand.h>

class OperatorController : frc2::SubsystemBase
{
    public:
        void test_command();
        frc2::InstantCommand test_command2{[this] {test_command();}, {this}};
        frc2::PrintCommand GODDAMNIT{"jkhkhkhhkhk"};
        OperatorController(Turret *turret_obj);
    private:
        frc::Joystick OperatorStick {OPERATOR_CONTROLLER};
        Turret* m_turret;
};
#endif