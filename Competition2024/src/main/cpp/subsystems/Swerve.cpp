#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

#include <ctre/phoenix6/configs/Configs.hpp>

#include "../../include/subsystems/Swerve.hpp"

using namespace frc;
using namespace ctre::phoenix6;

/// @brief Constructor for the Swerve class.
/// @param length - The length of the robot.
/// @param width - The width of the robot.
Swerve::Swerve(float length, float width)
{
    if(length == 0.0 || width == 0.0)
	   throw std::invalid_argument("Width and Length cannot be zero");

    this->m_gyro_offset = this->navx.GetRoll();

	LENGTH = length;
	WIDTH  = width;
	R = sqrt((LENGTH*LENGTH) + (WIDTH*WIDTH));

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

    // Magically (trigonomotry) returns a multidimensional array with angle rotation values and motor speeds
    // Giving this 4 agruments makes it field centric.
    double** SwerveMovement = Calculate(y, x, x2, gyro);

    // Incrementing is weird because the library uses a different motor order.
    // Drive Motors movement
    // Set is the speed value.
    DRIVE_MOTORS[0]->Set(SwerveMovement[1][0]);
    DRIVE_MOTORS[1]->Set(SwerveMovement[0][0]);
    DRIVE_MOTORS[2]->Set(SwerveMovement[2][0]);
    DRIVE_MOTORS[3]->Set(SwerveMovement[3][0]);

    // Angle Motors Movement I do not think that this will work, it will likely have to be put through the encoders, 
    // but it says that it uses "rotational value" which I'm not sure how to implement at this point.
    // SetPosition should set the angle motor to an angle. (not sure if its an encoder value or not)

    this->PID_CONTROLLERS[0]->SetReference(SwerveMovement[1][1] * SWERVE_WHEEL_COUNTS_PER_REVOLUTION, CANSparkMax::ControlType::kPosition);
    this->PID_CONTROLLERS[1]->SetReference(SwerveMovement[0][1] * SWERVE_WHEEL_COUNTS_PER_REVOLUTION, CANSparkMax::ControlType::kPosition);
    this->PID_CONTROLLERS[3]->SetReference(SwerveMovement[2][1] * SWERVE_WHEEL_COUNTS_PER_REVOLUTION, CANSparkMax::ControlType::kPosition);
    this->PID_CONTROLLERS[2]->SetReference(SwerveMovement[3][1] * SWERVE_WHEEL_COUNTS_PER_REVOLUTION, CANSparkMax::ControlType::kPosition);

    SmartDashboard::PutNumber("Driver Angle: ", gyro);

    SmartDashboard::PutNumber("Drive 0: ",    SwerveMovement[0][0]);
    SmartDashboard::PutNumber("Position 0: ", SwerveMovement[0][1]);

    SmartDashboard::PutNumber("Drive 1: ",    SwerveMovement[1][0]);
    SmartDashboard::PutNumber("Position 1: ", SwerveMovement[1][1]);

    SmartDashboard::PutNumber("Drive 2: ",    SwerveMovement[2][0]);
    SmartDashboard::PutNumber("Position 2: ", SwerveMovement[2][1]);

    SmartDashboard::PutNumber("Drive 3: ",    SwerveMovement[3][0]);
    SmartDashboard::PutNumber("Position 3: ", SwerveMovement[3][1]);
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
        this->ANGLE_ENCODERS[swerve_module]->SetPosition((this->ABS_ENCODERS[swerve_module].GetAbsolutePosition() / 360 * SWERVE_WHEEL_COUNTS_PER_REVOLUTION));
    }
}

// When field centric mode is disabled 'gyro' is ignored

/// @brief Method to ??? "Locks wheels so that the robot cannot move for better defence" - paraphrasing Dr. Maples
void Swerve::Toggle_X_Wheels()
{
    this->x_wheels = !this->x_wheels;

    SmartDashboard::PutBoolean("XWHEELS? ", this->x_wheels);
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

    y_deadzone = false;
    x_deadzone = false;
    y_move_abs = false;
}

double** Swerve::Calculate(double x, double y, double z, double angle)
{
	if (angle != -999.0)
	{
		angle       = angle * PI / 180;
		double temp =  x * cos(angle) + y * sin(angle);
		y           = -x * sin(angle) + y * cos(angle);
		x           = temp;
	}

	double A = y - z * (LENGTH / R);
	double B = y + z * (LENGTH / R);
	double C = x - z * (WIDTH  / R);
	double D = x + z * (WIDTH  / R);

	double wSpeed1 = sqrt(B * B + C * C);
	double wAngle1 = atan2(B, C) * 180 / PI;

	double wSpeed2 = sqrt(B * B + D * D);
	double wAngle2 = atan2(B, D) * 180 / PI;

	double wSpeed3 = sqrt(A * A + D * D);
	double wAngle3 = atan2(A, D) * 180 / PI;

	double wSpeed4 = sqrt(A * A + C * C);
	double wAngle4 = atan2(A, C) * 180 / PI;

	// Normalizes speeds so they're within the ranges of -1 to 1
	double maxSpeed = wSpeed1;
	if (wSpeed2 > maxSpeed) maxSpeed = wSpeed2;
	if (wSpeed3 > maxSpeed) maxSpeed = wSpeed3;
	if (wSpeed4 > maxSpeed) maxSpeed = wSpeed4;

	if (maxSpeed > 1)
	{
		wSpeed1 /= maxSpeed;
		wSpeed2 /= maxSpeed;
		wSpeed3 /= maxSpeed;
		wSpeed4 /= maxSpeed;
	}

	// Normalizes angles so they are within -1 to 1
	wAngle1 = wAngle1 / 360.0;
	wAngle2 = wAngle2 / 360.0;
	wAngle3 = wAngle3 / 360.0;
	wAngle4 = wAngle4 / 360.0;

	double temp[4][2] =	{	{wSpeed2, wAngle2},
							{wSpeed1, wAngle1},
							{wSpeed4, wAngle4},
							{wSpeed3, wAngle3}
						};
	double** output = CopyArray(temp);

	return output;
}

double** Swerve::CopyArray(double array[][2])
{
	double** output = new double*[4];
	for(int i = 0; i < 4; i++)
	{
		output[i] = new double[2];
		for(int k = 0; k < 2; k++)
		{
			output[i][k] = array[i][k];
		}
	}

	return output;
}
