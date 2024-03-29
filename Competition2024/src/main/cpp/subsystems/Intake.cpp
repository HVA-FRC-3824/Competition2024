#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/subsystems/Intake.hpp"

using namespace ctre::phoenix6;

/// @brief Constructor for the Intake class.
Intake::Intake()
{
    // Configure the intake motors
    configs::TalonFXConfiguration angle_motor_configuration{};

    // Configure current limits
    configs::MotionMagicConfigs &magic_motion = angle_motor_configuration.MotionMagic;
    magic_motion.MotionMagicCruiseVelocity = INTAKE_ACTUATION_VELOCITY;  // Rotations per second
    magic_motion.MotionMagicAcceleration   = INTAKE_ACTUATION_ACCELERATION;
    magic_motion.MotionMagicJerk           = INTAKE_ACTUATION_JERK;

    // Set slot 0 gains
    configs::Slot0Configs &slot0Configs = angle_motor_configuration.Slot0;
    slot0Configs.kP = INTAKE_ACTUATION_P;
    slot0Configs.kI = INTAKE_ACTUATION_I;
    slot0Configs.kD = INTAKE_ACTUATION_D;
    slot0Configs.kV = INTAKE_ACTUATION_V;
    slot0Configs.kS = INTAKE_ACTUATION_S;

    // Set the motor feedback
    configs::FeedbackConfigs &feedback_Configuration = angle_motor_configuration.Feedback;
    feedback_Configuration.SensorToMechanismRatio = INTAKE_ACTUATION_SENSOR_RATIO;

    // Apply the configuratoin 
    this->m_intake_angle_motor.GetConfigurator().Apply(angle_motor_configuration);

    // Set the current limit
    configs::CurrentLimitsConfigs currentLimitsConfigs{};
    currentLimitsConfigs.StatorCurrentLimit       = INTAKE_MAX_AMPERAGE;
    currentLimitsConfigs.StatorCurrentLimitEnable = true;
    this->m_intake_angle_motor.GetConfigurator().Apply(currentLimitsConfigs);

    // Configure the intake angle motor
    this->m_intake_angle_motor.SetPosition((units::angle::turn_t) INTAKE_START_POSITION);
    Set_Position(INTAKE_START_POSITION);
}

/// @brief Method called periodically every operator control packet.
void Intake::Robot_Periodic()
{
    frc::SmartDashboard::PutNumber("Intake State :", this->m_state);

    StatusSignal getPosition = this->m_intake_angle_motor.GetPosition();
    units::angle::turn_t position = getPosition.GetValue();
    frc::SmartDashboard::PutNumber("Intake Position: ", (uint) position);

    StatusSignal getVelocity = this->m_intake_angle_motor.GetVelocity();
    units::angular_velocity::turns_per_second_t velocity = getVelocity.GetValue();
    frc::SmartDashboard::PutNumber("Intake Velocity: ", (int) velocity);

    // Determine the intake is extending
    if (m_state == GoingToFeed)
    {
        // If close to the extend position, then set the intake state to extended
        if ((uint) position > INTAKE_FEED_POSITION - INTAKE_POSITION_OFFSET)
            m_state = Feed;
    }

    // Determine the intake is retracting
    if (m_state == GoingToAmp)
    {
        // If close to the retract position, then set the intake state to retracted
        if ((uint) position < INTAKE_AMP_POSITION + INTAKE_POSITION_OFFSET)
            m_state = Amp;
    }
}

/// @brief Method to set the intake motors to the specified set value (-1.0 to 1.0)
/// @param motor_set_value - The value to the motor output.
void Intake::Set_Roller_Motors(float motor_set_value)
{
    // Set the roller motor to the specified value
    m_intake_roller_motor.Set(motor_set_value);
}

/// @brief Method to extend the intake.
void Intake::MoveToFeed()
{
    // Determine if the intake is retracted
    if (m_state == Amp || m_state == Start)
    {
        std::cout << "Intake Extending\n";

        // Intake should now be extending
        m_state = GoingToFeed;

        // Set the intake angle to extended
        Set_Position(INTAKE_FEED_POSITION);
    }
}

/// @brief Method to retract the intake.
void Intake::MoveToAmp()
{
    if ((m_state == Feed))
    {
        std::cout << "Intake Retracting\n";

        // Intake should now be retracting
        m_state = GoingToAmp;

        // Set the intake angle to retracted
        Set_Position(INTAKE_AMP_POSITION);
    }
}

/// @brief Method to set the intake angle.
/// @param angle - The angle to set the intake.
void Intake::Set_Angle(float angle) 
{
    // Convert angle to encoder position.
    int encoder_position = Angle_To_Encoder(angle);

    std::cout << "angle: " << angle << ", encoder_position: " << encoder_position << ".\n";

    // Set the magic motion position to encoder position.
    Set_Position(encoder_position);
}

/// @brief Method to flip the intake subassembly from extend to retracted.
void Intake::Flip_Retraction()
{
    // Determine if the intake is retracted
    if (m_state == Amp || m_state == Start)
    {
        std::cout << "Intake Extending\n";

        // Intake should now be extending
        m_state = GoingToFeed;

        // Set the intake angle to extended
        Set_Position(INTAKE_FEED_POSITION);
    } 
    else if ((m_state == Feed))
    {
        std::cout << "*ntake Retracting\n";

        // Intake should now be retracting
        m_state = GoingToAmp;

        // Set the intake angle to retracted
        Set_Position(INTAKE_AMP_POSITION);
    }
}

/// @brief Method to set the intake subassembly to the specified position.
/// @param position - The position to set the intake subassembly.
void Intake::Set_Position(int position)
{
    std::cout << "SetControl:  " << position << "\n";

    // Set the move motion position setpoint 
    // Note: The position 0_tr in the control is overwritten using WithPosition
    controls::MotionMagicVoltage motionMagicVoltate{0_tr};
    this->m_intake_angle_motor.SetControl(motionMagicVoltate.WithPosition(position * 1_tr).WithSlot(0));
}


/// @brief Method to convert intake angle to encoder value.
/// @param angle - The angle to convert.
int Intake::Angle_To_Encoder(float angle)
{
    // Converting the intake angle to encoder value.
    return angle * ((INTAKE_ENCODER_90_DEGREES - INTAKE_ENCODER_0_DEGREES) / 90) + INTAKE_ENCODER_0_DEGREES;
}