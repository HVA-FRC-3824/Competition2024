#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/Intake.hpp"

using namespace ctre::phoenix6;

/// @brief Constructor for the Intake class.
Intake::Intake()
{
    configs::TalonFXConfiguration angle_motor_configuration{};

    // Configure current limits
    configs::MotionMagicConfigs &magic_motion = angle_motor_configuration.MotionMagic;
    magic_motion.MotionMagicCruiseVelocity = INTAKE_ACTUATION_VELOCITY;  // Rotations per second cruise
    magic_motion.MotionMagicAcceleration   = INTAKE_ACTUATION_ACCELERATION;
    magic_motion.MotionMagicJerk           = INTAKE_ACTUATION_JERK;

    // Set slot 0 gains
    configs::Slot0Configs &slot0Configs = angle_motor_configuration.Slot0;
    slot0Configs.kP = INTAKE_ACTUATION_P;
    slot0Configs.kI = INTAKE_ACTUATION_I;
    slot0Configs.kD = INTAKE_ACTUATION_D;
    slot0Configs.kV = INTAKE_ACTUATION_V;
    slot0Configs.kS = INTAKE_ACTUATION_S;

    configs::FeedbackConfigs &feedback_Configuration = angle_motor_configuration.Feedback;
    feedback_Configuration.SensorToMechanismRatio = INTAKE_ACTUATION_SENSOR_RATIO;

    // Apply the configuratoin
    this->m_intake_angle_motor.GetConfigurator().Apply(angle_motor_configuration);

    // Configure the intake angle motor
    this->m_intake_angle_motor.SetPosition((units::angle::turn_t) INTAKE_RETRACTED_POSITION);

    // Configure the follower motor
    controls::Follower follower{INTAKE_ACTUATION_CAN_ID, false};
    this->m_intake_angle_motor.SetControl(follower);
}

/// @brief Method called periodically every operator control packet.
void Intake::Robot_Periodic()
{
    frc::SmartDashboard::PutNumber("Intake State :", this->m_intake_state);

    ctre::phoenix6::StatusSignal getPosition = this->m_intake_angle_motor.GetPosition();
    units::angle::turn_t position = getPosition.GetValue();
    frc::SmartDashboard::PutNumber("Intake Position: ", (uint) position);

    ctre::phoenix6::StatusSignal getVelocity = this->m_intake_angle_motor.GetVelocity();
    units::angular_velocity::turns_per_second_t velocity = getVelocity.GetValue();
    frc::SmartDashboard::PutNumber("Intake Velocity: ", (int) velocity);

    // Determine the intake is extending
    if (m_intake_state == Extending)
    {
        // If close to the extend position, then set the intake state to extended
        if ((uint) position > INTAKE_EXTENDED_POSITION - INTAKE_OFFSET_POSITION_COUNT)
            m_intake_state = Extended;
    }

    // Determine the intake is retracting
    if (m_intake_state == Retracting)
    {
        // If close to the retract position, then set the intake state to retracted
        if ((uint) position < INTAKE_RETRACTED_POSITION + INTAKE_OFFSET_POSITION_COUNT)
            m_intake_state = Retracted;
    }
}

/// @brief Method to set the intake motors to the specified set value (-1.0 to 1.0)
/// @param motor_set_value - The value to the motor output.
void Intake::Set_Roller_Motors(float motor_set_value)
{
    // *** LIMIT MOTOR FOR BENCH TESTING ***
    // TODO: Remove
    if (motor_set_value > 0.2)
        motor_set_value = 0.2;
    if (motor_set_value < -0.2)
        motor_set_value = -0.2;

    // Set the roller motor to the specified value
    m_intake_roller_motor.Set(motor_set_value);
}

/// @brief Method to extend the intake.
void Intake::Extend_Intake()
{
    // Determine if the intake is retracted
    if (m_intake_state == Retracted)
    {
        std::cout << "Intake Extending\n";

        // Intake should now be extending
        m_intake_state = Extending;

        // Set the intake angle to extended
        Set_Actuate_Position(INTAKE_EXTENDED_POSITION);
    }
}

/// @brief Method to retract the intake.
void Intake::Retract_Intake()
{
    if ((m_intake_state == Extended))
    {
        std::cout << "Intake Retracting\n";

        // Intake should now be retracting
        m_intake_state = Retracting;

        // Set the intake angle to retracted
        Set_Actuate_Position(INTAKE_RETRACTED_POSITION);
    }
}

/// @brief Method to flip the intake subassembly from extend to retracted.
void Intake::Flip_Retraction()
{
    // Determine if the intake is retracted
    if (m_intake_state == Retracted)
    {
        std::cout << "Intake Extending\n";

        // Intake should now be extending
        m_intake_state = Extending;

        // Set the intake angle to extended
        Set_Actuate_Position(INTAKE_EXTENDED_POSITION);
    } 
    else if ((m_intake_state == Extended))
    {
        std::cout << "*ntake Retracting\n";

        // Intake should now be retracting
        m_intake_state = Retracting;

        // Set the intake angle to retracted
        Set_Actuate_Position(INTAKE_RETRACTED_POSITION);
    }
}

/// @brief Method to set the intake subassembly to the specified position.
/// @param position - The position to set the intake subassembly.
void Intake::Set_Actuate_Position(float position)
{
    std::cout << "SetControl:  " << position << "\n";

    // Set the move motion position setpoint 
    // Note: The position 0_tr in the control is overwritten using WithPosition
    controls::MotionMagicVoltage motionMagicVoltate{0_tr};
    this->m_intake_angle_motor.SetControl(motionMagicVoltate.WithPosition(position * 1_tr).WithSlot(0));
}