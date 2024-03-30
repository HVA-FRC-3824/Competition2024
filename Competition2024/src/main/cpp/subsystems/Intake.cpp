#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/subsystems/Intake.hpp"

using namespace frc;
using namespace ctre::phoenix6;

/// @brief Constructor for the Intake class.
Intake::Intake()
{
    // Configure the intake motors
    configs::TalonFXConfiguration angle_motor_configuration{};

    // Configure current limits
    configs::MotionMagicConfigs &magic_motion = angle_motor_configuration.MotionMagic;
    magic_motion.MotionMagicAcceleration   = INTAKE_ACTUATION_ACCELERATION;
    magic_motion.MotionMagicCruiseVelocity = INTAKE_ACTUATION_VELOCITY;
    magic_motion.MotionMagicJerk           = INTAKE_ACTUATION_JERK;

    // Set slot 0 gains
    configs::Slot0Configs &slot0Configs = angle_motor_configuration.Slot0;
    slot0Configs.kA = INTAKE_ACTUATION_A;
    slot0Configs.kD = INTAKE_ACTUATION_D;
    slot0Configs.kG = INTAKE_ACTUATION_G;
    slot0Configs.kI = INTAKE_ACTUATION_I;
    slot0Configs.kP = INTAKE_ACTUATION_P;
    slot0Configs.kS = INTAKE_ACTUATION_S;
    slot0Configs.kV = INTAKE_ACTUATION_V;

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

    // Set the motor direction
    configs::MotorOutputConfigs motorOutputConfigs{};
    motorOutputConfigs.Inverted    = true;
    motorOutputConfigs.NeutralMode = signals::NeutralModeValue::Brake;
    this->m_intake_angle_motor.GetConfigurator().Apply(motorOutputConfigs);

    // Configure the intake angle motor to start at the specified encover value
    this->m_intake_angle_motor.SetPosition((units::angle::turn_t) INTAKE_START_POSITION);

    // Enable the intake motor PID
    Set_Position(INTAKE_START_POSITION);
}

/// @brief Method called periodically every operator control packet.
void Intake::Robot_Periodic()
{
    SmartDashboard::PutNumber("Intake State: ", this->m_state);

    StatusSignal getPosition = this->m_intake_angle_motor.GetPosition();
    units::angle::turn_t position = getPosition.GetValue();
    SmartDashboard::PutNumber("Intake Position: ", (double) position);

    StatusSignal getVelocity = this->m_intake_angle_motor.GetVelocity();
    units::angular_velocity::turns_per_second_t velocity = getVelocity.GetValue();
    SmartDashboard::PutNumber("Intake Velocity: ", (double) velocity);

    // Determine the intake is extending
    if (m_state == GoingToFeed)
    {
        // If close to the extend position, then set the intake state to extended
        if ((double) position > m_intake_feed_position - INTAKE_POSITION_REACHED_POSITION)
            m_state = Feed;
    }

    // Determine the intake is retracting
    if (m_state == GoingToAmp)
    {
        // If close to the retract position, then set the intake state to retracted
        if ((double) position < m_intake_amp_position + INTAKE_POSITION_REACHED_POSITION)
            m_state = Amp;
    }
}

/// @brief Method to set the intake motors to the specified set value (-1.0 to 1.0)
/// @param motor_set_value - The value to the motor output.
void Intake::Set_Roller_Motors(double motor_set_value)
{
    // Set the roller motor to the specified value
    m_intake_roller_motor.Set(motor_set_value);
}

/// @brief Method to extend the intake.
void Intake::MoveToFeed()
{
    // Determine if the intake is retracted
    //if (m_state == Amp || m_state == Start)
    {
        // Intake should now be extending
        m_state = GoingToFeed;

        // Set the intake angle to extended
        Set_Position(m_intake_feed_position);
    }
}

/// @brief Method to retract the intake.
void Intake::MoveToAmp()
{
    //if ((m_state == Feed) || (m_state == Climb))
    {
        // Intake should now be retracting
        m_state = GoingToAmp;

        // Set the intake angle to retracted
        Set_Position(m_intake_amp_position);
    }
}

/// @brief Method to get out of the way for climb.
void Intake::MoveToClimb()
{
    // Intake to be set away from climb.
    m_state = Climb;

    // Set the motor position away from Climb
    Set_Position(INTAKE_CLIMB_POSITION);
}

/// @brief Method to flip the intake subassembly from extend to retracted.
void Intake::Flip_Retraction()
{
    // Determine if the intake is retracted
    if (m_state == Amp || m_state == Start)
    {
        // Intake should now be extending
        m_state = GoingToFeed;

        // Set the intake angle to extended
        Set_Position(m_intake_feed_position);
    } 
    else if ((m_state == Feed) || (m_state == Climb))
    {
        // Intake should now be retracting
        m_state = GoingToAmp;

        // Set the intake angle to retracted
        Set_Position(m_intake_amp_position);
    }
}

/// @brief Method to add/subtrat the specified offset to the intake offset to account for belt slippage.
/// @param offset - The offset to add/subtract to the intake offset.
void Intake::AddIntakeOffset(double offset)
{
    // Add the intake offset to the amp, feed and preset set position (subtract if the offset if negative)
    m_intake_amp_position  += offset;
    m_intake_feed_position += offset;
    m_present_set_position += offset;

    SmartDashboard::PutNumber("m_intake_amp_position: ",   m_intake_amp_position);
    SmartDashboard::PutNumber("Iintake_feed_position: ", m_intake_feed_position);
    SmartDashboard::PutNumber("m_present_set_position: ",  m_present_set_position);

    // Update the intak positoin
    Set_Position(m_present_set_position);
}

/// @brief Method to set the intake subassembly to the specified position.
/// @param position - The position to set the intake subassembly.
void Intake::Set_Position(double position)
{
    // Calculate the set position based on the specified position and the intake offset
    m_present_set_position = position;

    // Set the move motion position setpoint 
    // Note: The position 0_tr in the control is overwritten using WithPosition
    controls::MotionMagicVoltage motionMagicVoltate{0_tr};
    this->m_intake_angle_motor.SetControl(motionMagicVoltate.WithPosition(m_present_set_position * 1_tr).WithSlot(0));
}
