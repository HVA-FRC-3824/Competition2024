#include <iostream>
#include <math.h>
#include <string>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <ctre/phoenix6/configs/Configs.hpp>
#include "../../include/subsystems/Swerve.hpp"

/// @brief Constructor for the Swerve class.
/// @param length - The length of the robot.
/// @param width - The width of the robot.
Swerve::Swerve(float length, float width)
{
    // Remember the chassis length and width
    this->chassis_info.length = length;
    this->chassis_info.width = width;

    // Clear the swerver module angle and power memory
    clear_swerve_memory();

    // Initialize the swerve motors
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    {
        //### ANGLE MOTORS ###

        // Set current limit
        this->ANGLE_MOTORS[swerve_module]->SetSmartCurrentLimit(SWERVE_MAX_AMPERAGE);

        // Turn on brake coast mode, snappier
        this->ANGLE_MOTORS[swerve_module]->SetIdleMode(CANSparkMax::IdleMode::kBrake);

        // Swerve wheel PID contrllers
        this->PID_CONTROLLERS[swerve_module] = new SparkMaxPIDController(this->ANGLE_MOTORS[swerve_module]->GetPIDController());
        this->PID_CONTROLLERS[swerve_module]->SetP(SWERVE_P);
        this->PID_CONTROLLERS[swerve_module]->SetI(SWERVE_I);
        this->PID_CONTROLLERS[swerve_module]->SetD(SWERVE_D);

        // Get real relative encoders
        this->ANGLE_ENCODERS[swerve_module] = new SparkRelativeEncoder(this->ANGLE_MOTORS[swerve_module]->GetEncoder());
        this->ABS_ENCODERS[swerve_module].ConfigAbsoluteSensorRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);

        // Burn flash everytime
        this->ANGLE_MOTORS[swerve_module]->BurnFlash();

        //### DRIVE MOTORS ###
        ctre::phoenix6::configs::Slot0Configs slot0Configs{};
        slot0Configs.kP = SWERVE_P; // An error of 0.5 rotations results in 12 V output
        slot0Configs.kI = SWERVE_I; // no output for integrated error
        slot0Configs.kD = SWERVE_D; // A velocity of 1 rps results in 0.1 V output

        this->DRIVE_MOTORS[swerve_module]->GetConfigurator().Apply(slot0Configs);
    }
};

/// @brief The method to drive the robot.
/// @param y - The operator y stick value.
/// @param x - The operator x stick value.
/// @param x2 - The operator second x stick value.
/// @param gyro - The robot gyro heading.
void Swerve::Drive(float y, float x, float x2, float gyro)
{
    // Lock wheels in x position
    if (this->x_wheels)
    {
        for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
           this->DRIVE_MOTORS[swerve_module]->Set(0);
    
        // Initialize the PID controllers
        this->PID_CONTROLLERS[0]->SetReference(-7.875, CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[1]->SetReference( 7.875, CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[2]->SetReference( 2.625, CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[3]->SetReference(-2.625, CANSparkMax::ControlType::kPosition);
        return;
    }

    // Correct/Sanitize our inputs
    deadzone_correction(&x, &y, &x2);

    // Generate our math and store in dest struct to be converted / used
    calculate_wheel_information(&this->math_dest,this->chassis_info, y, x, x2);

    // Find the percent to max angle (180 or -180) and then multiple by the counts required to get to that required angle.
    // Equivalent to x / SWERVE_WHEEL_COUNTS_PER_REVOLUTION = y / 360 where y is angle and x is raw sensor units for the encoder
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    { 
        this->raw_usable[swerve_module] = ((this->math_dest.wheel_angle[swerve_module] / 360) * SWERVE_WHEEL_COUNTS_PER_REVOLUTION); 
    }

    print_swerve_math(this->math_dest);

    // Only run our motors once everything is calculated
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    {
        frc::SmartDashboard::PutNumber(fmt::v10::format("CANCODER_POS_{}: ", swerve_module), this->ABS_ENCODERS[swerve_module].GetAbsolutePosition());
        //frc::SmartDashboard::PutNumber(fmt::v9::format("SPARKMAX_POS_{}: ", i), this->ANGLE_ENCODERS[i]->GetPosition());
        this->DRIVE_MOTORS[swerve_module]->Set(this->math_dest.wheel_speeds[swerve_module]);

        if(use_old)
        {
            this->PID_CONTROLLERS[swerve_module]->SetReference(this->last_units[swerve_module], CANSparkMax::ControlType::kPosition);
            //std::cout << i << "Actual: " << this->ANGLE_ENCODERS[i]->GetPosition() << " Old_Desired: " << this->last_units[i] <<"\n";
        } else 
        {
            this->PID_CONTROLLERS[swerve_module]->SetReference(this->raw_usable[swerve_module], CANSparkMax::ControlType::kPosition);
            //std::cout << i << "Actual: " << this->ANGLE_ENCODERS[i]->GetPosition() << " Desired: " << this->raw_usable[i] <<"\n";
            this->last_units[swerve_module] = this->raw_usable[swerve_module];
        }

        // Clear "sticky" values that are stuck in memory, if the robot is receiving input this doesn't matter anyways.
        // Only affects the robot when stopped!!
        this->math_dest.wheel_speeds[swerve_module] = 0;
    }
    this->use_old = false;
}

/// @brief Method to toggle the field centricity.
/// @return The new field centricity state.
bool Swerve::Toggle_Field_Centricity()
{
    // Toggle the field centricity
    this->field_centered = !this->field_centered;
    
    // Return the new field centricity state
    return this->field_centered;
}

/// @brief Method to set the wheens to the aboslute position.
void Swerve::Snap_Wheels_To_Absolute_Position()
{
    for (int swerve_module = 0; swerve_module < 4; swerve_module++)
    {
        // Snap to abs then set to zero
        //this->PID_CONTROLLERS[i]->SetReference(-(this->ABS_ENCODERS[i].GetAbsolutePosition()/360)*SWERVE_WHEEL_COUNTS_PER_REVOLUTION,CANSparkMax::ControlType::kPosition);
        this->ANGLE_ENCODERS[swerve_module]->SetPosition((this->ABS_ENCODERS[swerve_module].GetAbsolutePosition() / 360) * SWERVE_WHEEL_COUNTS_PER_REVOLUTION);
    }
}

// When field centric mode is disabled 'gyro' is ignored

/// @brief Method to ??? 
void Swerve::Toggle_X_Wheels()
{
    this->x_wheels = !this->x_wheels;

    frc::SmartDashboard::PutBoolean("XWHEELS? ", this->x_wheels);
}

/// @brief Method to print the wheel speeds and angles.
/// @param wheel_information - The wheel information. 
void Swerve::print_swerve_math(wheel_info wheel_information)
{
    std::cout << "\n";
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    {
        //std::cout << wheel_information.wheel_speeds[swerve_module] << " SPEED " << swerve_module << "\n";
        //std::cout << wheel_information.wheel_angle[swerve_module]  << " ANGLE " << swerve_module << "\n";
    }
}

/// @brief Method to clear the swerve_module state memory arrays.
void Swerve::clear_swerve_memory()
{
    for(int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    {
        this->math_dest.wheel_angle[swerve_module]  = 0;
        this->math_dest.wheel_speeds[swerve_module] = 0;
        this->raw_usable[swerve_module]             = 0;
        this->last_units[swerve_module]             = 0; 
    }
}

/// @brief Method to calculate the new wheel drive and rotation values.
/// @param dest - The requested move parameters.
/// @param cons - Robot width and length.
/// @param fwd - The move forward value.
/// @param str - The move strafe value.
/// @param rotate - The move rotate value.
void Swerve::calculate_wheel_information(wheel_info *dest, struct size_constants cons, float fwd, float str, float rotate)
{
    float forward = fwd;
	float strafe  = str;
    float gyro    = navx.GetYaw();

    frc::SmartDashboard::PutNumber("Gyro Input: ", gyro);

	// If field centric take in account the gyro
    float temp =  (fwd * cosf(gyro * ANTIMAGIC_NUMBER)) + (str * sinf(gyro * ANTIMAGIC_NUMBER));
    strafe     = -(fwd * sinf(gyro * ANTIMAGIC_NUMBER)) + (str * cosf(gyro * ANTIMAGIC_NUMBER));
	forward    = temp;

	float R = sqrtf((cons.length * cons.length) + (cons.width * cons.width));

	float A = strafe  - rotate * (cons.length / R);
	float B = strafe  + rotate * (cons.length / R);
	float C = forward - rotate * (cons.width  / R);
	float D = forward + rotate * (cons.width  / R);

	// Fill out our dest struct
	float a_sqrd = A * A;
	float b_sqrd = B * B;
	float c_sqrd = C * C;
	float d_sqrd = D * D;

	dest->wheel_speeds[0] = sqrtf((b_sqrd) + (c_sqrd));
	dest->wheel_speeds[1] = sqrtf((b_sqrd) + (d_sqrd));
	dest->wheel_speeds[2] = sqrtf((a_sqrd) + (d_sqrd));
	dest->wheel_speeds[3] = sqrtf((a_sqrd) + (c_sqrd));

	float max = dest->wheel_speeds[0];

	// Do normalisation on the wheel speeds, so that they are 0.0 <-> +1.0
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
	{
		if (dest->wheel_speeds[swerve_module] > max) 
        {
            max = dest->wheel_speeds[swerve_module];
        }
	}

	if (max > 1)
	{
        for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
        {
            dest->wheel_speeds[swerve_module] = dest->wheel_speeds[swerve_module] / max; 
        }
	}

	dest->wheel_angle[0] = atan2f(B,C) * MAGIC_NUMBER;
	dest->wheel_angle[1] = atan2f(B,D) * MAGIC_NUMBER;
	dest->wheel_angle[2] = atan2f(A,D) * MAGIC_NUMBER;
	dest->wheel_angle[3] = atan2f(A,C) * MAGIC_NUMBER;

    // Instead of flippig 180 just reverse our speeds, this might be a problem and needs testing
    for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
    {
        if (dest->wheel_angle[swerve_module] == 180)
        {
            dest->wheel_speeds[swerve_module] = dest->wheel_speeds[swerve_module] * -1; 
            continue;
        }

        if (dest->wheel_angle[swerve_module] < -90)
        {
            dest->wheel_angle[swerve_module] += 180;
            dest->wheel_speeds[swerve_module] = dest->wheel_speeds[swerve_module] * -1;
        }

        if (dest->wheel_angle[swerve_module] > 90)
        {
            dest->wheel_angle[swerve_module] -= 180;
            dest->wheel_speeds[swerve_module] = dest->wheel_speeds[swerve_module] * -1;
        }
    } 
	return;
}

/// @brief Method to create dead zones for the controller joysticks.
/// @param x - Pointer to the x stick value to return the value used.
/// @param y - Pointer to the y stick value to return the value used.
/// @param x2 - Pointer to the second x stick value to return the value used.
void Swerve::deadzone_correction(float *x, float *y, float *x2)
{
    // Pass in the values and they are corrected
    bool y_deadzone = false;
    bool x_deadzone = false;
    bool y_move_abs = false;

    // Ignore our deadzone and fix the moving forward issue
    if (*y < DEADZONE_THRESHOLD && *y > -DEADZONE_THRESHOLD)
    {
        *y = 0;
        y_deadzone = true;
    }   

    if (*x < DEADZONE_THRESHOLD && *x > -DEADZONE_THRESHOLD)
    {
        *x = 0;
        x_deadzone = true;
        if (!y_deadzone)
        {
            // Sets so that the true forward value is used incase of no strafing
            y_move_abs = true;
        }
    } 

    if (*x2 < DEADZONE_THRESHOLD && *x2 > -DEADZONE_THRESHOLD)
    {
        *x2 = 0;
    }

    // Allows for forward movement even when X is in deadzone
    if (y_move_abs)
    {
        for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
        {
            this->math_dest.wheel_speeds[swerve_module] = abs(*y);
        }
    }

    // IF DEADZONE IS REACHED WE USED LAST KNOWN VALUE, WE SHOULD NEVER RESET ANGLE (according to dr hart)

    // Allows for sideways movement even when Y is in deadzone
    if (y_deadzone)
    {
        for (int swerve_module = 0; swerve_module < SWERVE_MODULES; swerve_module++)
        {
            this->math_dest.wheel_speeds[swerve_module] = abs(*x);
        }
    }

    // Re Use old units instead of spinning again
    if (y_deadzone && x_deadzone)
    {
        this->use_old = true;
    }

    y_deadzone = false;
    x_deadzone = false;
    y_move_abs = false;
}
