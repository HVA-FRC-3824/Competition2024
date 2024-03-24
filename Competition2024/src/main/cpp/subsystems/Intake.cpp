#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "../../include/subsystems/Intake.hpp"

using namespace ctre::phoenix6;

/// @brief Constructor for the Intake class.
Intake::Intake()
{
    configs::TalonFXConfiguration configuration{};

    /* Configure current limits */
    configs::MotionMagicConfigs &magic_motion = configuration.MotionMagic;
    magic_motion.MotionMagicCruiseVelocity = INTAKE_ACTUATION_VELOCITY;  // Rotations per second cruise
    magic_motion.MotionMagicAcceleration   = INTAKE_ACTUATION_ACCELERATION;
    magic_motion.MotionMagicJerk           = INTAKE_ACTUATION_JERK;

    // set slot 0 gains
    configs::Slot0Configs &slot0Configs = configuration.Slot0;
    slot0Configs.kP = INTAKE_ACTUATION_P;
    slot0Configs.kI = INTAKE_ACTUATION_I;
    slot0Configs.kD = INTAKE_ACTUATION_D;
    slot0Configs.kV = INTAKE_ACTUATION_V;
    slot0Configs.kS = INTAKE_ACTUATION_S;  // Approximately 0.1V to get the mechanism moving

    configs::FeedbackConfigs &feedback_Configuration = configuration.Feedback;
    feedback_Configuration.SensorToMechanismRatio = 1;

    // Apply the configuratoin
    this->m_intake_angle_motor.GetConfigurator().Apply(configuration);

    // configure the intake motors
    this->m_intake_angle_motor.SetPosition((units::angle::turn_t) INTAKE_RETRACTED_POSITION);
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
void Intake::Drive_Rollers(float motor_set_value)
{
    // Set the roller motor to the specified value
    m_intake_roller_motor.Set(motor_set_value);
}

/// @brief Method to extend the intake.
void Intake::ExtendIntake()
{
    // Determine if the intake is retracted
    if (m_intake_state == Retracted)
    {
        std::cout << "*** Intake Extending\n";

        // Intake should now be extending
        m_intake_state = Extending;

        // Set the intake angle to extended
        Intake_Actuate_Point(INTAKE_EXTENDED_POSITION);
    }
}

/// @brief Method to retract the intake.
void Intake::RetractIntake()
{
    if ((m_intake_state == Extended))
    {
        std::cout << "*** Intake Retracting\n";

        // Intake should now be retracting
        m_intake_state = Retracting;

        // Set the intake angle to retracted
        Intake_Actuate_Point(INTAKE_RETRACTED_POSITION);
    }
}

/// @brief Method to flip the intake subassembly from extend to retracted.
void Intake::Flip_Retraction()
{
    // Determine if the intake is retracted
    if (m_intake_state == Retracted)
    {
        std::cout << "*** Intake Extending\n";

        // Intake should now be extending
        m_intake_state = Extending;

        // Set the intake angle to extended
        Intake_Actuate_Point(INTAKE_EXTENDED_POSITION);
    } 
    else if ((m_intake_state == Extended))
    {
        std::cout << "*** Intake Retracting\n";

        // Intake should now be retracting
        m_intake_state = Retracting;

        // Set the intake angle to retracted
        Intake_Actuate_Point(INTAKE_RETRACTED_POSITION);
    }
}

/// @brief Method to set the intake subassembly to the specified position.
/// @param position - The position to set the intake subassembly.
void Intake::Intake_Actuate_Point(float position)
{
    std::cout << "*** SetControl:  " << position << "\n";

    // Set the move motion position setpoint 
    controls::MotionMagicVoltage m_mmReq{1_tr};
    this->m_intake_angle_motor.SetControl(m_mmReq.WithPosition(position * 1_tr).WithSlot(0));
}