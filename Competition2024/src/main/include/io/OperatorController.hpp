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
        /// @brief Variable to keep track of the periodic calls
        long periodic_counter            = 0;

        /// @brief Variable to debounce the intake lower button 
        long intake_lower_debounce_count = 0;

        /// @brief Variable to debounce the intake raise button   
        long intake_raise_debounce_count = 0;

        /// @brief Pointer to the intake class to allow calling intake methods.
        Intake *m_intake;

        /// @brief Pointer to the climb class to allow calling climb methods.
        Climb *m_climb;

        /// @brief The operator controller (joystick).
        frc::Joystick m_operator_joystick{OPERATOR_CONTROLLER};
};
