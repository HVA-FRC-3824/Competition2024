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

    if (!this->field_centricity)
        gyro = -999;

    // Magically (trigonomotry) returns a multidimensional array with angle rotation values and motor speeds
    // Giving this 4 agruments makes it field centric.
    double** SwerveMovement = getMotorControl(x,y,x2,gyro);

    // Incrementing is weird because the library uses a different motor order.
    // Drive Motors movement
    // Set is the speed value.
    
    DRIVE_MOTORS[0]->Set(SwerveMovement[1][0] * 0.3);
    DRIVE_MOTORS[1]->Set(SwerveMovement[0][0] * 0.3);
    DRIVE_MOTORS[2]->Set(SwerveMovement[2][0] * 0.3);
    DRIVE_MOTORS[3]->Set(SwerveMovement[3][0] * 0.3);
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

void Swerve::Toggle_Fast_Wheels() 
{
    this->fast_wheels = !this->fast_wheels;
    
    SmartDashboard::PutBoolean("Fast Wheels? ", this->fast_wheels);
}

void Swerve::Toggle_Field_Centricity() 
{
    this->field_centricity = !this->field_centricity;
    
    SmartDashboard::PutBoolean("Fast Wheels? ", this->fast_wheels);
}


double** Swerve::getMotorControl(double x, double y, double x2, double gyro) 
{
    // Set backup angle variables.
    this->tAngle1 = this->ANGLE_ENCODERS[1]->GetPosition();
    this->tAngle2 = this->ANGLE_ENCODERS[2]->GetPosition();
    this->tAngle3 = this->ANGLE_ENCODERS[3]->GetPosition();
    this->tAngle4 = this->ANGLE_ENCODERS[4]->GetPosition();
    // Converting them to be usable
	this->ANGLE_ENCODERS[1]->SetPositionConversionFactor(tAngle1);
	this->ANGLE_ENCODERS[2]->SetPositionConversionFactor(tAngle2);
	this->ANGLE_ENCODERS[3]->SetPositionConversionFactor(tAngle3);
	this->ANGLE_ENCODERS[4]->SetPositionConversionFactor(tAngle4);

    // Calculate the angles and speeds for the robot.
    double** angles_speeds = Calculate(y, x, x2, gyro);

    // Pass into function that fixes some things.
    //angles_speeds = angleProtection((double**)angles_speeds, x2);

    // if (this->fast_wheels)
    //     for (int i = 0; i > 4; i++)
    //         angles_speeds[i][0] = angles_speeds[i][0] * 0.3;

    return angles_speeds;


}

/// @breif Makes it so that while turning it both doesn't go straight back to 0 and it takes a quicker route to the desired angle.
double** Swerve::angleProtection(double** APmotorMovements, double x2)
{
    // converts motorMovements into previously used angles (I just reused code)
    double wAngle1 = APmotorMovements[0][1];
    double wAngle2 = APmotorMovements[1][1];
    double wAngle3 = APmotorMovements[2][1];
    double wAngle4 = APmotorMovements[3][1];

    // // Makes code into useable degrees
    // wAngle1 = wAngle1 * 360;
    // wAngle2 = wAngle2 * 360;
    // wAngle3 = wAngle3 * 360;
    // wAngle4 = wAngle4 * 360;

	// c angle; current angle.
    double cAngle1 = this->ANGLE_ENCODERS[1]->GetPosition();
    double cAngle2 = this->ANGLE_ENCODERS[2]->GetPosition();
    double cAngle3 = this->ANGLE_ENCODERS[3]->GetPosition();
    double cAngle4 = this->ANGLE_ENCODERS[4]->GetPosition();
	this->ANGLE_ENCODERS[1]->SetPositionConversionFactor(cAngle1);
	this->ANGLE_ENCODERS[2]->SetPositionConversionFactor(cAngle2);
	this->ANGLE_ENCODERS[3]->SetPositionConversionFactor(cAngle3);
	this->ANGLE_ENCODERS[4]->SetPositionConversionFactor(cAngle4);
	
	// ex: cAngle1 == 90; wAngle1 == 270; 270 - 90 == 180 (this should be a positive value since we want it to go up)
	// ex: cAngle1 == 10; wAngle1 == 270; 270 - 10 == 260 (this is slow since it would be faster to go -100 degrees, we want a negative number)
	//  to solve this we can do the following calculation

	//  If the wanted angle is above 180 then we will probably want it to be negative, if we don't that's fine and we will fix that.
    //  Arbitrary high number
    double iwAngle1 = 1000*1000;
    double iwAngle2 = 1000*1000;
    double iwAngle3 = 1000*1000;
    double iwAngle4 = 1000*1000;
    // Actual Solution
	if (wAngle1 > 180) {iwAngle1 = 360 - wAngle1;}
    if (wAngle2 > 180) {iwAngle2 = 360 - wAngle2;}
    if (wAngle3 > 180) {iwAngle3 = 360 - wAngle3;}
    if (wAngle4 > 180) {iwAngle4 = 360 - wAngle4;}


	// This checks if it would be faster to go negative or Positive, then it decides which is better.
    // This takes the absolute of each (by squaring them then taking the root) then compares
	if (abs(iwAngle1 - cAngle1) 
      < abs(wAngle1 - cAngle1)) {wAngle1 = iwAngle1;}

    if (abs(iwAngle2 - cAngle2) 
      < abs(wAngle2 - cAngle2)) {wAngle2 = iwAngle2;}

    if (abs(iwAngle3 - cAngle3) 
      < abs(wAngle3 - cAngle3)) {wAngle3 = iwAngle3;}

    if (abs(iwAngle4 - cAngle4) 
      < abs(wAngle4 - cAngle4)) {wAngle4 = iwAngle4;}

	// Given the previous example; this should set a negative value to wAngle1, which is then passed to the motors
	// and we can repeat this for every subsequent motor.

	//  Now we reach another problem where the Angle motors still go back to zero.
	//  For this I'll add an exception where it can never reach the 0 default state unless its already there.
    // if (x2 != 0) 
	// {
	// 	this->tAngle1 = wAngle1;
    //     this->tAngle2 = wAngle2;
    //     this->tAngle3 = wAngle3;
    //     this->tAngle4 = wAngle4;
	// } else if (x2 == 0)
	// {
    //     wAngle1 = this->tAngle1;
    //     wAngle2 = this->tAngle2;
    //     wAngle3 = this->tAngle3;
    //     wAngle4 = this->tAngle4;
	// }

    // Makes code into 1 to -1 motor drivers.
    wAngle1 = wAngle1 / 360;
    wAngle2 = wAngle2 / 360;
    wAngle3 = wAngle3 / 360;
    wAngle4 = wAngle4 / 360;

    // Sets to array then outputs it
    APmotorMovements[0][1] = wAngle1;
    APmotorMovements[1][1] = wAngle2;
    APmotorMovements[2][1] = wAngle3;
    APmotorMovements[3][1] = wAngle4;
    
    return APmotorMovements;
}

double** Swerve::Calculate(double x, double y, double z, double angle)
{
	/// @todo Have angle variables in a double* array, speed doesn't *need* this but it would be consistent.
	
	// Check for Field or Robot Centricity, if you put any number for "angle" it becomes Field.
	if (angle != -999.0)
	{
		angle       = angle * PI / 180;
		double temp =  x * cos(angle) + y * sin(angle);
		y           = -x * sin(angle) + y * cos(angle);
		x           = temp;
	}

	// Not sure what this is for, but these are the variables used for math
	double A = y - z * (LENGTH / R);
	double B = y + z * (LENGTH / R);
	double C = x - z * (WIDTH  / R);
	double D = x + z * (WIDTH  / R);

	// Calculate Target Speeds and Angles
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

    if (this->fast_wheels)
    {
        wSpeed1 = wSpeed1 / 2;
        wSpeed2 = wSpeed2 / 2;
        wSpeed3 = wSpeed3 / 2;
        wSpeed4 = wSpeed4 / 2;
    }

	// Normalizes angles so they are within -1 to 1
	// wAngle1 = wAngle1 / 360.0;
	// wAngle2 = wAngle2 / 360.0;
	// wAngle3 = wAngle3 / 360.0;
	// wAngle4 = wAngle4 / 360.0;

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
