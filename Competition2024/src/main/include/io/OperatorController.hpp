#ifndef OPERATOR_CONTROLLER_H
#define OPERATOR_CONTROLLER_H
#include "../Constants.h"
#include "../subsystems/Turret.hpp"
#include "../subsystems/Launcher.hpp"
#include "AHRS.h"
#include <frc2/command/SubsystemBase.h>
#include <frc/Joystick.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/PrintCommand.h>

/* As of 1/29/24 operator only needs to control: Launcher, Turret, Intake, NAVX */
/* Each said subsystem will have a "soft lock" that prevents operator input (controlled by operator ofc) 
   that stops all movement (prevents accidental inputs) and allows computer to run it's magic
   However, this lock usually works independent from a full hard lock within the subsystems (ie. Turret) that
   prevents ALL changes */

class OperatorController : frc2::SubsystemBase
{
    public:
        void test_command();
        void reset_gyro();
        frc2::InstantCommand test_command2{[this] {test_command();}, {this}};
        frc2::InstantCommand reset_gyro_command{[this] {reset_gyro();}, {this}};
        frc2::PrintCommand GODDAMNIT{"jkhkhkhhkhk"};
        OperatorController(Turret *turret_obj, AHRS *ahrs_obj, Launcher *launcher_obj);
    private:
        frc::Joystick OperatorStick {OPERATOR_CONTROLLER};
        frc2::CommandPtr last_damn_try();
        Turret* m_turret;
        AHRS* ahrs;
        Launcher* m_launcher;
};
#endif