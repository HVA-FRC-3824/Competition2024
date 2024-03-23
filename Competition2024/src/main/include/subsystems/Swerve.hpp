#pragma once

#include "../Constants.hpp"

#include "AHRS.h"
#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <ctre/phoenix/sensors/CANCoder.h>
#include <ctre/phoenix6/TalonFX.hpp>

#define SWERVE_MODULES 4

struct wheel_information
{
	/* Order of wheels as in the real world: */
	/* 0 = front right, 1 = front left       */
	/* 2 = rear left,   3 = rear right       */

	float wheel_speeds[4]; 
	float wheel_angle[4];
};

typedef struct wheel_information wheel_info;

struct size_constants
{
	float length;
	float width;
};

using namespace rev;
using namespace ctre::phoenix6::hardware;

class Swerve : frc2::SubsystemBase
{
    public:
        /// @brief Indicates the field centricity of the swerve drive.
        bool field_centered = false;

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

        /// @brief Method to toggle the field centricity.
        /// @return The new field centricity state.
        bool Toggle_Field_Centricity();

        /// @brief Method to set the wheens to the aboslute position.
        void Snap_Wheels_To_Absolute_Position();

        /// @brief /// @brief Method to ??? 
        void Toggle_X_Wheels();

        /// @brief Create an attitude and heading reference system (AHRS).
        AHRS navx{frc::SerialPort::SerialPort::Port::kMXP};

    private:
        /// @brief Method to print the wheel speeds and angles.
        /// @param wheel_information - The wheel information. 
        void print_swerve_math(wheel_info wheel_information);            // debug

        /// @brief Method to clear the swerve_module state memory arrays.
        void clear_swerve_memory(); 
         
        /// @brief Method to calculate the new wheel drive and rotation values.
        /// @param dest - The requested move parameters.
        /// @param cons - Robot width and length.
        /// @param fwd - The move forward value.
        /// @param str - The move strafe value.
        /// @param rotate - The move rotate value.
        void calculate_wheel_information(wheel_info *dest, struct size_constants cons, float fwd, float str, float rotate);

        // @brief Method to create dead zones for the controller joysticks.
        /// @param x - Pointer to the x stick value to return the value used.
        /// @param y - Pointer to the y stick value to return the value used.
        /// @param x2 - Pointer to the second x stick value to return the value used.
        void deadzone_correction(float *x, float *y, float *x2);

        double last_units[4]; 

        bool use_old  = false;
        bool x_wheels = false;

        // Save point for speed and angle values
        wheel_info math_dest;

        // Width and Height
        struct size_constants chassis_info;

        // Stores the raw usable units for the motor controllers
        double raw_usable[4];

        // Motor bank. Follows the format in the math_dest 
        //    0 = front right, 1 = front left 
        //    2 = rear left,   3 = rear right

        //delcration of drive motors under namespace ctre::phoenix6::hardware
        //TalonFX FR_MOTOR_M(int deviceId = FR_M_CAN_ID, std::string canbus = "rio");
        //TalonFX FL_MOTOR_M(int deviceId = FL_M_CAN_ID, std::string canbus = "rio");
        //TalonFX RL_MOTOR_M(int deviceId = RL_M_CAN_ID, std::string canbus = "rio");
        //TalonFX RR_MOTOR_M(int deviceId = RR_M_CAN_ID, std::string canbus = "rio");

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

        SparkRelativeEncoder* ANGLE_ENCODERS[4];
        SparkMaxPIDController* PID_CONTROLLERS[4];

        ctre::phoenix::sensors::CANCoder ABS_ENCODERS[4]
        {
            41,
            43,
            40,
            42
        };
};
