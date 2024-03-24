#pragma once

#include "../Constants.hpp"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix6/TalonFX.hpp>
#include "rev/CANSparkMax.h"

class Intake : frc2::SubsystemBase
{
    public: 
        /// @brief Bolean to indicate if the operator is locked out of the intake angle.
        bool intake_locked = false;

        /// @brief Constructor for the Intake class.
        Intake();

        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Method to set the intake motors to the specified set value (-1.0 to 1.0)
        /// @param motor_set_value - The value to the motor output.
        void Set_Roller_Motors(float motor_set_value);

        /// @brief Method to extend the intake.
        void Extend_Intake();

        /// @brief Method to retract the intake.
        void Retract_Intake();

        /// @brief Method to flip the intake subassembly from extend to retracted.
        void Flip_Retraction();

    private:
        enum Intake_State
        {
            Retracted,
            Extending,
            Extended,
            Retracting
        };

        /// @brief The intake state.
        Intake_State m_intake_state = Retracted;

        /// @brief The intake roller motor.
        rev::CANSparkMax m_intake_roller_motor{INTAKE_MOTOR_CAN_ID, rev::CANSparkLowLevel::MotorType::kBrushless};

        /// @brief The intake angle motor.
        ctre::phoenix6::hardware::TalonFX m_intake_angle_motor{INTAKE_ACTUATION_CAN_ID, "rio"};

        /// @brief The intake angle motor follower.
        ctre::phoenix6::hardware::TalonFX m_intake_angle_follower_motor{INTAKE_ACTUATION_FOLLOWER_CAN_ID, "rio"};

        /// @brief Method to set the intake subassembly to the specified position.
        /// @param position - The position to set the intake subassembly.
        void Set_Actuate_Position(float position);
};
