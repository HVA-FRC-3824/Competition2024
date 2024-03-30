#pragma once

#include <unistd.h>

#include "Swerve.hpp"
#include "Intake.hpp"

/// @brief Class to support autonomous commands.
class AutoCommands
{
    public:
        /// @brief Constructor for the AutoCommands class.
        /// @param swerve - Pointer to the robot swerve subsystem.
        /// @param intake - Pointer to the robot intake subsystem.
        AutoCommands(Swerve *swerve, Intake *intake);

        /// @brief Autonomous command to do nothing.
        void Auto_Do_Nothing();

        /// @brief Autonomous command to drive forward.
        void Auto_Move();

        /// @brief  Autonomous comand to place a note in the Amp, and move out of the tape.
        /// @param move_after_amp - Bool to indacate if it should move after AMP.
        void Auto_Place_Amp_And_Move(bool move_after_amp);

    private:
        /// @brief Pointer to the robot swerve subsystem.
        Swerve *m_swerve;

        /// @brief Pointer to the robot intake subsystem.
        Intake *m_intake;
};
