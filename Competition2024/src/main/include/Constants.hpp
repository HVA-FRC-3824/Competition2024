#pragma once

// Not on CAN Ids and naming, anything above or on the turret should be prefixed 2#, such as 20 for the physical turret
// IDs should be numbered starting from the turret up, incrementing each time
// Anything below should be 1# or 0#

// Intake variables
#define INTAKE_MOTOR_CAN_ID                16
#define INTAKE_ACTUATION_CAN_ID            17
#define INTAKE_ACTUATION_P              0.075
#define INTAKE_ACTUATION_I              0.000
#define INTAKE_ACTUATION_D              0.000
#define INTAKE_BOTTOM_POINT             40483

// Actuation variables
#define ACTUATION_MOTOR_CAN_ID             24
#define ACTUATION_ENCODER_CAN_ID            0
#define ACTUATION_P                      20.0
#define ACTUATION_I                       1.0
#define ACTUATION_D                       0.0
#define ACTUATION_INDEX_ANGLE             -20
#define ACTUATION_UFR                  806400
#define MAX_LOWER_ANGLE                   -60
#define A_VERTICAL                       1000
#define A_HORIZONTAL                     2000
#define ACTUATION_SETPOINT                  0
#define ACTUATION_PRESENT                   0

// Swerve variables
// CAN IDS
#define FR_M_CAN_ID                         7
#define FL_M_CAN_ID                        12
#define RL_M_CAN_ID                        13
#define RR_M_CAN_ID                        10

#define FR_A_CAN_ID                         8
#define FL_A_CAN_ID                         4
#define RL_A_CAN_ID                         6
#define RR_A_CAN_ID                         9

// Climber Variable
#define CLIMBER_DEPLOY_RIGHT_CAN_ID        31
#define CLIMBER_DEPLOY_LEFT_CAN_ID         30
#define CLIMBER_UP_POWER                  0.5
#define CLIMBER_DOWN_POWER               -0.5

// Hardware Constants
#define SWERVE_MAX_AMPERAGE                30

// Swerve Constants
#define DEADZONE_THRESHOLD                0.1   // Raise to counter joystick drift this number is on a controller to controller basis

// The amount of raw sensor units to complete one full rotation
#define SWERVE_WHEEL_COUNTS_PER_REVOLUTION 21

// PID Values for the motorcontrollers
#define SWERVE_P                        0.700
#define SWERVE_I                        0.000
#define SWERVE_D                        0.001

// PID Values for angle controller
#define ANGLES_SWERVE_P                  0.05
#define ANGLES_SWERVE_I                  0.00
#define ANGLES_SWERVE_D                  0.00

// 180 / Pi
#define MAGIC_NUMBER                 57.29577f
#define ANTIMAGIC_NUMBER              0.01745f

// Global constants
#define OPERATOR_CONTROLLER                 0
#define DRIVER_CONTROLLER                   1

// Command constants

// Explanation on commands: 
// Commands are created in OperatorController.cpp as standard methods, written as if they will be popped into another thread
// those commands are then given an ID as below (see C_INTAKE_OB) and in CommandScheduler.cpp it is added in the switch statement
// in OperatorController.cpp the flags are set in the memory shared between the two causing the command to run

#define C_ACTIVE                         0xFF
#define C_KILL                           0x20
#define C_NONE                           0x10
#define C_RUN                            0xF0
#define C_INACTIVE                       0x00

#define C_INTAKE_OB                      0x01
#define C_AMP_OB                         0x02

/// @brief 
struct CommandParameters
{
    unsigned char command = C_NONE;
    unsigned char state   = C_INACTIVE;
    unsigned char action  = C_NONE;
};
