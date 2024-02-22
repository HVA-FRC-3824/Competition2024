#ifndef OPERATOR_CONTROLLER_H
#define OPERATOR_CONTROLLER_H
#include "../Constants.h"
#include "../subsystems/Turret.hpp"
#include "../subsystems/Launcher.hpp"
#include "../subsystems/Intake.hpp"
#include "../subsystems/Actuation.hpp"
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

/* Hexadecimal bc i'm a little autistic */
/* These are the states the controller can be, A_MODE and B_MODE are two types of modes on the controller
   O_TEST is the test mode for resetting navx or testing set actuation points

   ALL Buttons declared should be designed in the cpp file TODO: move these to constants


   ALL CONTROLS ARE LISTED AND DEFINED AS IN THE README, ANY BUTTON CHANGES MUST BE REPLICATED TO THE README
*/
#define O_LOCKED    0x00
#define O_SOFT_LOCK 0xff
#define O_ACTIVE    0xf0
#define A_MODE      0x0a
#define B_MODE      0x0b
#define O_TEST      0x01

class OperatorController : frc2::SubsystemBase
{
    public:
        void reset_gyro();
        void one_button_intake();
        void robo_periodic(); /* Links to teleop periodic */
        OperatorController(cmd_share *share, Turret *turret_obj, AHRS *ahrs_obj, Launcher *launcher_obj, Intake *intake_obj, Actuation *actuation_obj);
        cmd_share *shared;
    private:
        frc::Joystick OperatorStick {OPERATOR_CONTROLLER};
        Intake* m_intake;
        Turret* m_turret;
        AHRS* ahrs;
        Launcher* m_launcher;
        Actuation *m_actuation;
        uint8_t state = 0xf0;
        uint8_t mode = 0x0a;

        bool command_active = false;
};
#endif