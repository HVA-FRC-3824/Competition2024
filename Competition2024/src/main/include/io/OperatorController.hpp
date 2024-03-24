#pragma once 

#include "../Constants.hpp"

#include "AHRS.h"

#include <frc2/command/SubsystemBase.h>
#include <frc/Joystick.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/PrintCommand.h>

#include "../subsystems/Intake.hpp"
#include "../subsystems/Climb.hpp"

// Each said subsystem will have a "soft lock" that prevents operator input (controlled by operator ofc) 
// that stops all movement (prevents accidental inputs) and allows computer to run it's magic
// However, this lock usually works independent from a full hard lock within the subsystems (ie. Turret) that
// prevents ALL changes

#define O_SOFT_LOCK 0xFF
#define O_ACTIVE    0xF0

class OperatorController : frc2::SubsystemBase
{
    public:
        /// @brief Constructor for the OperatorController class.
        /// @param intake - Pointer to the intake class to allow calling intake methods.
        /// @param climb - Pointer to the climb class to allow calling climb methods.
        OperatorController(Intake *intake, Climb *climb);

        /// @brief Method called periodically every dirver/operator control packet.
        void Robot_Periodic(); 

        /// @brief Method to run the intake.
        void One_Button_Intake();

        /// @brief Method to place in the amp.
        void One_Button_Amp();

    private:
        /// @brief The state of reading the operator joystick inputs (either active or locked out)
        uint8_t m_state = O_ACTIVE;

        /// @brief Pointer to the intake class to allow calling intake methods.
        Intake *m_intake;

        /// @brief Pointer to the climb class to allow calling climb methods.
        Climb *m_climb;

        /// @brief The operator controller (joystick).
        frc::Joystick m_operator_joystick {OPERATOR_CONTROLLER};
};
