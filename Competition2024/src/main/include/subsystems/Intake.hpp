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
        bool retracted = true;
        
        /// @brief Constructor for the Intake class.
        Intake();

        /// @brief Method called periodically every operator control packet.
        void Robot_Periodic();

        /// @brief Method to run the actuate motors.
        /// @param motor_set_value - The actuate motor set value.
        void Intake_Actuate_Simple(float motor_set_value);

        /// @brief Method to set the actuate subassembly to the specified angle.
        /// @param angle - The angle to set the acturate subassembly.
        void Intake_Actuate_Point(float angle);

        /// @brief Method to flip the intake subassembly from extend to retracted.
        void Flip_Retraction();
 
    private:
        //ctre::phoenix::motorcontrol::can::WPI_TalonSRX intake_motor{INTAKE_MOTOR_CAN_ID};
        //rev::CANSparkMax m_actuation_motor{INTAKE_ACTUATION_CAN_ID, rev::CANSparkLowLevel::MotorType::kBrushless};
        
        /// @brief The intake angle motor.
        ctre::phoenix6::hardware::TalonFX m_intake_angle_motor{INTAKE_ACTUATION_CAN_ID, "rio"};

        /// @brief The intake toller motor.
        rev::CANSparkMax m_intake_roller_motor{INTAKE_MOTOR_CAN_ID, rev::CANSparkLowLevel::MotorType::kBrushless};
};
