#ifndef SWERVE_H
#define SWERVE_H

#include "../Constants.h"

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>

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

class Swerve : frc2::SubsystemBase
{
    public:
        int setUnits = 0; /* test value */
        bool field_centered = false;
        Swerve(float length, float width);
        void drive(float x, float y, float x2, float gyro); // gyro is ignored when field_centered is false
        void print_swerve_math(wheel_info math); // debug
        bool toggle_field_centricity(); // returns changed state
        void calculate_wheel_information(wheel_info *dest, struct size_constants cons, float fwd, float str, float rotate, uint8_t field_centric, float gyro);
        void clear_swerve_memory(); // call when values are stuckington
    private:
        void deadzone_correction(float *x, float *y, float *x2);
        double last_units[4]; 
        bool use_old = false;

        /* Save point for speed and angle values */
        wheel_info math_dest;

        /* Width and Height */
        struct size_constants chassis_info;

        /* Stores the raw usable units for the motor controllers */
        double raw_usable[4];

        /* Motor bank. Follows the format in the math_dest 
            0 = front right, 1 = front left 
            2 = rear left,   3 = rear right */
        
        CANSparkMax FR_MOTOR_M{FR_M,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax FL_MOTOR_M{FL_M,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax RL_MOTOR_M{RL_M,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax RR_MOTOR_M{RR_M,CANSparkMaxLowLevel::MotorType::kBrushless};

        CANSparkMax* DRIVE_MOTORS[4] = 
        {
            &FR_MOTOR_M,
            &FL_MOTOR_M,
            &RL_MOTOR_M,
            &RR_MOTOR_M
        };

        CANSparkMax FR_MOTOR_A{FR_A,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax FL_MOTOR_A{FL_A,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax RL_MOTOR_A{RL_A,CANSparkMaxLowLevel::MotorType::kBrushless};
        CANSparkMax RR_MOTOR_A{RR_A,CANSparkMaxLowLevel::MotorType::kBrushless};

        CANSparkMax* ANGLE_MOTORS[4] = 
        {
            &FR_MOTOR_A,
            &FL_MOTOR_A,
            &RL_MOTOR_A,
            &RR_MOTOR_A
        };

        SparkMaxRelativeEncoder* ANGLE_ENCODERS[4];
        SparkMaxPIDController* PID_CONTROLLERS[4];
};

#endif