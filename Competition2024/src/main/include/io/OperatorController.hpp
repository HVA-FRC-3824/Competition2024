#pragma once 

#include "../Constants.hpp"

#include <frc/Joystick.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/button/JoystickButton.h>

#include "../subsystems/Intake.hpp"
#include "../subsystems/Climb.hpp"

class OperatorController : frc2::SubsystemBase
{
    public:
        /// @brief Constructor for the OperatorController class.
        /// @param intake - Pointer to the intake class to allow calling intake methods.
        /// @param climb - Pointer to the climb class to allow calling climb methods.
        OperatorController(Intake *intake, Climb *climb);

        /// @brief Method called periodically every dirver/operator control packet.
        void Robot_Periodic(); 

    private:
        /// @brief Pointer to the intake class to allow calling intake methods.
        Intake *m_intake;

        /// @brief Pointer to the climb class to allow calling climb methods.
        Climb *m_climb;

        /// @brief The operator controller (joystick).
        frc::Joystick m_operator_joystick{OPERATOR_CONTROLLER};
};
