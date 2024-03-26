#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/subsystems/Climb.hpp"

using namespace ctre::phoenix6;

/// @brief Constructor for the Climb class.
Climb::Climb()
{
    // Configure the climber motors
    configs::TalonFXConfiguration climb_motor_configuration{};

    // Configure current limits
    configs::MotionMagicConfigs &magic_motion = climb_motor_configuration.MotionMagic;
    magic_motion.MotionMagicCruiseVelocity = CLIMBER_ACTUATION_VELOCITY;  // Rotations per second
    magic_motion.MotionMagicAcceleration   = CLIMBER_ACTUATION_ACCELERATION;
    magic_motion.MotionMagicJerk           = CLIMBER_ACTUATION_JERK;

    // Set slot 0 gains
    configs::Slot0Configs &slot0Configs = climb_motor_configuration.Slot0;
    slot0Configs.kP = CLIMBER_ACTUATION_P;
    slot0Configs.kI = CLIMBER_ACTUATION_I;
    slot0Configs.kD = CLIMBER_ACTUATION_D;
    slot0Configs.kV = CLIMBER_ACTUATION_V;
    slot0Configs.kS = CLIMBER_ACTUATION_S;

    configs::FeedbackConfigs &feedback_Configuration = climb_motor_configuration.Feedback;
    feedback_Configuration.SensorToMechanismRatio = CLIMBER_ACTUATION_SENSOR_RATIO;

    // Set the climber motor configurations
    for (int motor = 0; motor < CLIMBER_MOTORS; motor++)
    {
       // Apply the configuratoin
       this->m_climb_motors[motor]->GetConfigurator().Apply(climb_motor_configuration);

       // Configure the CLIMBER angle motor
       this->m_climb_motors[motor]->SetPosition((units::angle::turn_t) CLIMBER_RETRACTED_POSITION);
    }
}

/// @brief Method called periodically every operator control packet.
void Climb::Robot_Periodic()
{
    frc::SmartDashboard::PutNumber("Climb State :", this->m_state);

    StatusSignal getPosition = this->m_climb_motors[0]->GetPosition();
    units::angle::turn_t position = getPosition.GetValue();
    frc::SmartDashboard::PutNumber("Climb Position: ", (uint) position);

    StatusSignal getVelocity = this->m_climb_motors[0]->GetVelocity();
    units::angular_velocity::turns_per_second_t velocity = getVelocity.GetValue();
    frc::SmartDashboard::PutNumber("Climb Velocity: ", (int) velocity);

    // Determine the climber is extending
    if (m_state == Extending)
    {
        // If close to the extend position, then set the climb state to extended
        if ((uint) position > CLIMBER_EXTENDED_POSITION - CLIMBER_OFFSET_POSITION_COUNT)
            m_state = Extended;
    }

    // Determine the climber is retracting
    if (m_state == Retracting)
    {
        // If close to the retract position, then set the climb state to retracted
        if ((uint) position < CLIMBER_RETRACTED_POSITION + CLIMBER_OFFSET_POSITION_COUNT)
            m_state = Retracted;
    }
}

/// @brief Method to extend the climber.
void Climb::Extend()
{
    // Determine if the climber is retracted
    if (m_state == Retracted)
    {
        std::cout << "Climber Extending\n";

        // Climber should now be extending
        m_state = Extending;

        // Set the climber to extended
        Set_Position(CLIMBER_EXTENDED_POSITION);
    }
}

/// @brief Method to retract the climber.
void Climb::Retract()
{
    if ((m_state == Extended))
    {
        std::cout << "Climber Retracting\n";

        // Climber should now be retracting
        m_state = Retracting;

        // Set the climber to retracted
        Set_Position(CLIMBER_RETRACTED_POSITION);
    }
}

/// @brief Method to set the climber hooks to the specified position.
/// @param position - The position to set the climber hooks.
void Climb::Set_Position(float position)
{
    std::cout << "SetControl:  " << position - CLIMBER_RETRACTED_POSITION << "\n";

    // Set the move motion position setpoint 
    // Note: The position 0_tr in the control is overwritten using WithPosition
    controls::MotionMagicVoltage motionMagicVoltate{0_tr};
    for (int motor = 0; motor < CLIMBER_MOTORS; motor++)
        this->m_climb_motors[motor]->SetControl(motionMagicVoltate.WithPosition(position * 1_tr).WithSlot(0));
}
