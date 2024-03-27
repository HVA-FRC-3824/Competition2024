#pragma once

#include "../Constants.hpp"

#include <frc2/command/SubsystemBase.h>

#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <ctre/phoenix6/TalonFX.hpp>

#include <rev/SparkmaxAbsoluteEncoder.h>
#include "rev/CANSparkMax.h"

using namespace ctre::phoenix6::hardware;
using namespace rev;

class Intake : frc2::SubsystemBase
{
    public: 
        /// @brief Constructor for the Intake class.
        Intake();

        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Method to set the intake motors to the specified set value (-1.0 to 1.0)
        /// @param motor_set_value - The value to the motor output.
        void Set_Roller_Motors(float motor_set_value);

        /// @brief Method to extend the intake.
        void Extend();

        /// @brief Method to retract the intake.
        void Retract();

        /// @brief Method to set the intake angle.
        /// @param angle - The angle to set the intake.
        void Set_Angle(float angle);

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
        Intake_State m_state = Retracted;

        ///@brief The absolute encoder.
        //SparkAbsoluteEncoder AbsoluteEncoder(const SparkAbsoluteEncoder & rhs);

        /// @brief The intake roller motor.
        CANSparkMax m_intake_roller_motor{INTAKE_ROLLER_MOTOR_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};

        /// @brief The intake angle motor.
        TalonFX m_intake_angle_motor{INTAKE_ACTUATION_CAN_ID, CANBUS_NAME};

        /// @brief Method to set the intake subassembly to the specified position.
        /// @param position - The position to set the intake subassembly.
        void Set_Position(int position);

        /// @brief Method to convert intake angle to encoder value.
        /// @param angle - The angle to convert.
        int Angle_To_Encoder(float angle);
};
