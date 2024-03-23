#pragma once

#include "../Constants.hpp"
#include "Swerve.hpp"
#include "Actuation.hpp"
#include <frc/controller/PIDController.h>

#include <unistd.h>

/// @brief Class to support autonomous commands.
class AutoCommands
{
    public:
        /// @brief Constructor for the AutoCommands class.
        /// @param swerve - Pointer to the robot swerve subsystem.
        /// @param actuation - Pointer to the robot actuation subsystem.
        AutoCommands(Swerve *swerve, Actuation *actuation);

        /// @brief Autonomous command to do nothing.
        void Auto_Do_Nothing();

        /// @brief Autonomous command to drive forward.
        void Auto_Move();

        //frc::PIDController ANGLE_CONTROLLER{ANGLES_SWERVE_P, ANGLES_SWERVE_I, ANGLES_SWERVE_D};

    private:
        /// @brief Pointer to the robot swerve subsystem.
        Swerve *m_swerve;

        /// @brief Pointer to the robot actuation subsystem.
        Actuation *m_actuation;
};
