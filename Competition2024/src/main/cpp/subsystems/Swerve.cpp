#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

#include <ctre/phoenix6/configs/Configs.hpp>

#include "../../include/subsystems/Swerve.hpp"
#include "main/SwerveLib/RobotDriveSwerve.h"

using namespace frc;
using namespace ctre::phoenix6;

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

        configs::TalonFXConfiguration swerve_motor_configuration{};

        //### DRIVE MOTORS ###  TODO: Add SWERVE_MAX_AMPERACE and brake mode
        configs::Slot0Configs slot0Configs = swerve_motor_configuration.Slot0;
        slot0Configs.kP = SWERVE_P; // An error of 0.5 rotations results in 12 V output
        slot0Configs.kI = SWERVE_I; // no output for integrated error
        slot0Configs.kD = SWERVE_D; // A velocity of 1 rps results in 0.1 V output

        this->DRIVE_MOTORS[swerve_module]->GetConfigurator().Apply(swerve_motor_configuration);
        
        // Set the current limit
        configs::CurrentLimitsConfigs currentLimitsConfigs{};
        currentLimitsConfigs.StatorCurrentLimit       = SWERVE_MAX_AMPERAGE;
        currentLimitsConfigs.StatorCurrentLimitEnable = true;
        this->DRIVE_MOTORS[swerve_module]->GetConfigurator().Apply(currentLimitsConfigs);
    }
};

/// @brief The method to drive the robot.
/// @param y - The operator y stick value - forwards.
/// @param x - The operator x stick value - strafe.
/// @param x2 - The operator second x stick value - yaw
/// @param gyro - The robot gyro heading.
/// 
 
void Swerve::Drive(float y, float x, float x2, float gyro)
{
    // Correct/Sanitize our inputs
    deadzone_correction(&x, &y, &x2);

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

    //new code using lib
    
    // This fuction produces the "R" of the robot, likely means Radius
    SwerveMath(CHASSIS_LENGTH,CHASSIS_WIDTH);

    // Magically (trigonomotry) returns a multidimensional array with angle rotation values and motor speeds
    // Giving this 4 agruments makes it field centric.
    double** SwerveMovement = Calculate(y,x,x2,gyro);

    // Incrementing is weird because the library uses a different motor order.
    // Drive Motors movement
    DRIVE_MOTORS[0]->Set(SwerveMovement[1][0]);
    DRIVE_MOTORS[1]->Set(SwerveMovement[0][0]);
    DRIVE_MOTORS[2]->Set(SwerveMovement[2][0]);
    DRIVE_MOTORS[3]->Set(SwerveMovement[3][0]);
    // Angle Motors Movement I do not think that this will work, it will likely have to be put through the encoders, 
    // but it says that it uses "rotational value" which I'm not sure how to implement at this point.
    ANGLE_MOTORS[0]->Set(SwerveMovement[1][1]);
    ANGLE_MOTORS[1]->Set(SwerveMovement[0][1]);
    ANGLE_MOTORS[2]->Set(SwerveMovement[2][1]);
    ANGLE_MOTORS[3]->Set(SwerveMovement[3][1]);
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

/// @brief Method to ??? "Locks wheels so that the robot cannot move for better defence" - paraphrasing Dr. Maples
void Swerve::Toggle_X_Wheels()
{
    this->x_wheels = !this->x_wheels;

    SmartDashboard::PutBoolean("XWHEELS? ", this->x_wheels);
}

/// @brief Method to print the wheel speeds and angles.
/// @param wheel_information - The wheel information. 
void Swerve::print_swerve_math(wheel_info wheel_information)
{
    //std::cout << "\n";
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

    SmartDashboard::PutNumber("Gyro Input: ", gyro);

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
