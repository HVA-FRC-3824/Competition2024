#include <frc/smartdashboard/SmartDashboard.h>

#include "../../include/subsystems/Climb.hpp"

using namespace frc;

/// @brief Constructor for the Climb class.
Climb::Climb()
{

}

/// @brief Method called periodically every operator control packet.
void Climb::Robot_Periodic()
{
    SmartDashboard::PutBoolean("Climb Right Enable: ", m_motor_enable[0]);
    SmartDashboard::PutBoolean("Climb Left Enable: ",  m_motor_enable[1]);
}

/// @brief Method to set the motor output.
/// @param output - The motor output percentage (-1.0 to 1.0)
void Climb::Set_Motor_Output(double output)
{
    // Set the motor output 
    for (int motor = 0; motor < CLIMBER_MOTORS; motor++)
    {
        // Ensure the motor is enabled
        if (m_motor_enable[motor] == true)
           this->m_climb_motors[motor]->Set(ControlMode::PercentOutput, output);
        else
           this->m_climb_motors[motor]->Set(ControlMode::PercentOutput, 0.0);

        // Get the motor current
        double current = this->m_climb_motors[motor]->GetOutputCurrent();

        // Determine which motor is being checked
        if (motor == 0)
           SmartDashboard::PutNumber("Climb Right Current: ", current);
        else
           SmartDashboard::PutNumber("Climb Left Current: ", current);

        // Disable the motor is climb motor current is exceeded (i.e., reached end of climb)
        if (current > CLIMB_MAXIMUM_OUTPUT_CURRENT)
            m_motor_enable[motor] = false;
    }
}

/// @brief Methos to enable the climber motors.
void Climb::Enable_Climber_Motors(bool state)
{
    // Enable the motors 
    for (int motor = 0; motor < CLIMBER_MOTORS; motor++)
        m_motor_enable[motor] = state;    
}
