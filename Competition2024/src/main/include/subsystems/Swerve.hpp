#pragma once

#include "../Constants.hpp"

#include "AHRS.h"

#include <frc2/command/SubsystemBase.h>

#include <rev/CANSparkMax.h>
#include <ctre/phoenix/sensors/CANCoder.h>
#include <ctre/phoenix6/TalonFX.hpp>

using namespace ctre::phoenix6::hardware;
using namespace rev;

class Swerve : frc2::SubsystemBase
{
    public:
        /// @brief Indicates the field centricity of the swerve drive.
        // bool field_centered = true;

        /// @brief Constructor for the Swerve class.
        /// @param length - The length of the robot.
        /// @param width - The width of the robot.
        Swerve(float length, float width);

        /// @brief The method to drive the robot.
        /// @param y - The operator y stick value.
        /// @param x - The operator x stick value.
        /// @param x2 - The operator second x stick value.
        /// @param gyro - The robot gyro heading.
        void Drive(float y, float x, float x2, float gyro);  // gyro is ignored when field_centered is false

        /// @brief Method to set the wheens to the aboslute position.
        void Snap_Wheels_To_Absolute_Position();

        /// @brief Method to "x" the wheels, prevents the robot from moving. 
        void Toggle_X_Wheels();

	    /// @brief Makes the wheels go 30% of their normal speed.
	    void Toggle_Fast_Wheels();

        /// @brief field centricity turns on and off
        void Toggle_Field_Centricity();

        /// @brief Create an attitude and heading reference system (AHRS).
        AHRS navx{frc::SerialPort::SerialPort::Port::kMXP};

    private:
        bool x_wheels = false;
	    bool fast_wheels = true;
        bool field_centricity = false;

         double tAngle1 = 0;
         double tAngle2 = 0;
         double tAngle3 = 0;
         double tAngle4 = 0;

        ctre::phoenix6::hardware::TalonFX FR_MOTOR_M{FR_M_CAN_ID, CANBUS_NAME};
        ctre::phoenix6::hardware::TalonFX FL_MOTOR_M{FL_M_CAN_ID, CANBUS_NAME};
        ctre::phoenix6::hardware::TalonFX RL_MOTOR_M{RL_M_CAN_ID, CANBUS_NAME};
        ctre::phoenix6::hardware::TalonFX RR_MOTOR_M{RR_M_CAN_ID, CANBUS_NAME};

        ctre::phoenix6::hardware::TalonFX *DRIVE_MOTORS[4] = {
            &FR_MOTOR_M,
            &FL_MOTOR_M,
            &RL_MOTOR_M,
            &RR_MOTOR_M
        };

        CANSparkMax FR_MOTOR_A{FR_A_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};
        CANSparkMax FL_MOTOR_A{FL_A_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};
        CANSparkMax RL_MOTOR_A{RL_A_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};
        CANSparkMax RR_MOTOR_A{RR_A_CAN_ID, CANSparkLowLevel::MotorType::kBrushless};

        CANSparkMax *ANGLE_MOTORS[4] = 
        {
            &FR_MOTOR_A,
            &FL_MOTOR_A,
            &RL_MOTOR_A,
            &RR_MOTOR_A
        };

        SparkRelativeEncoder  *ANGLE_ENCODERS[4];
        SparkMaxPIDController *PID_CONTROLLERS[4];

        ctre::phoenix::sensors::CANCoder ABS_ENCODERS[4]
        {
            FR_E_CAN_ID,
            FL_E_CAN_ID,
            RL_E_CAN_ID,
            RR_E_CAN_ID
        };


    double** getMotorControl(double x,double y,double x2,double gyro);

    double** angleProtection(double** APmotorMovements, double x2);


    static constexpr double NO_ANGLE = -999.0;
	static constexpr double PI       = acos(-1.0);

    /*
    * Uses foward speed, strafing speed, and rotational speed values to calculate
    * the required angle and speed for each wheel.  An angle can also be given so
    * that field centric mode can be used.  If no angle is given (or equal to -999)
    *  robot centric will be used.
    *
    * FORWARD:  positive value = forward movement,   negative value = backward movement
    * STRAFE:   positive value = right direction,    negative value = left direction
    * ROTATION: positive value = clockwise rotation, negative value = counterclockwise rotation
    *
    * Method outputs an array of speed and rotation value for each wheel.
    * 		0					1
    * 	0	Front Left Speed	Front Left Angle
    * 	1	Front Right Speed	Front Right Angle
    * 	2	Rear Left Speed		Rear Left Angle
    * 	3	Rear Right Speed	Rear Right Angle
    */
    double** Calculate(double fwd, double str, double rot, double angle = -999);    

	/*
	 * Copies the speed and angle values into a pointer in order to be used by
	 * the enclosures
	 */
	double** CopyArray(double array[][2]);

	double LENGTH, WIDTH;
	double R;
};
