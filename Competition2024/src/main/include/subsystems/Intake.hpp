#pragma once

#include "../Constants.hpp"

#include <frc2/command/SubsystemBase.h>

#include <ctre/phoenix6/TalonFX.hpp>

#include "rev/CANSparkMax.h"

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
        void Set_Roller_Motors(double motor_set_value);

        /// @brief Method to extend the intake.
        void MoveToFeed();

        /// @brief Method to retract the intake.
        void MoveToAmp();

        /// @brief Method to flip the intake subassembly from extend to retracted.
        void Flip_Retraction();

        /// @brief Method to add/subtrat the specified offset to the intake offset to account for belt slippage.
        /// @param offset - The offset to add/subtract to the intake offset.
        void AddIntakeOffset(double offset);

    private:
        enum Intake_State
        {
            Start,
            Amp,
            GoingToFeed,
            Feed,
            GoingToAmp
        };

        double m_present_set_position = 0.0;                    // The preset intake set position
        double m_intake_amp_position  = INTAKE_AMP_POSITION;    // The intake amplifier position
        double m_intake_feed_position = INTAKE_FEED_POSITION;   // The intake feed position

        /// @brief The intake state.
        Intake_State m_state = Start;

        /// @brief The intake roller motor.
        CANSparkMax m_intake_roller_motor{INTAKE_ROLLER_MOTOR_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};

        /// @brief The intake angle motor.
        ctre::phoenix6::hardware::TalonFX m_intake_angle_motor{INTAKE_ACTUATION_CAN_ID, CANBUS_NAME};

        /// @brief Method to set the intake subassembly to the specified position.
        /// @param position - The position to set the intake subassembly.
        void Set_Position(double position);
};
