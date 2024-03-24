#pragma once

// CAN bus name (specifically for TalonFX)
#define CANBUS_NAME                        "rio"

// ### Controller constants ###
#define OPERATOR_CONTROLLER                    0
#define DRIVER_CONTROLLER                      1

// #### Swerve variables ####
#define SWERVE_MODULES                         4

// CAN IDs Driver Motors
#define FR_M_CAN_ID                            7
#define FL_M_CAN_ID                           12
#define RL_M_CAN_ID                           13
#define RR_M_CAN_ID                           10

// CAN IDs Angle Motors
#define FR_A_CAN_ID                            8
#define FL_A_CAN_ID                            4
#define RL_A_CAN_ID                            6
#define RR_A_CAN_ID                            9

// Hardware Constants
#define SWERVE_MAX_AMPERAGE                   30

// Swerve Constants
#define DEADZONE_THRESHOLD                   0.1   // Raise to counter joystick drift this number is on a controller to controller basis

// The amount of raw sensor units to complete one full rotation
#define SWERVE_WHEEL_COUNTS_PER_REVOLUTION    21

// PID Values for the motorcontrollers
#define SWERVE_P                           0.700
#define SWERVE_I                           0.000
#define SWERVE_D                           0.001

// PID Values for angle controller
#define ANGLES_SWERVE_P                     0.05
#define ANGLES_SWERVE_I                     0.00
#define ANGLES_SWERVE_D                     0.00

// ### Intake variables ###
#define INTAKE_MOTOR_CAN_ID                   20

#define INTAKE_ACTUATION_CAN_ID               21
#define INTAKE_ACTUATION_FOLLOWER_CAN_ID      22
#define INTAKE_ACTUATION_SENSOR_RATIO          1  // Adjust per robot (could set to 1 and just use encoder counts)
#define INTAKE_ACTUATION_P                 5.000
#define INTAKE_ACTUATION_I                 0.500
#define INTAKE_ACTUATION_D                 0.000
#define INTAKE_ACTUATION_V                 0.000
#define INTAKE_ACTUATION_S                 0.000  // Approximately 0.1V to get the mechanism moving

#define INTAKE_ACTUATION_VELOCITY             30
#define INTAKE_ACTUATION_ACCELERATION         20
#define INTAKE_ACTUATION_JERK                  5

#define INTAKE_RETRACTED_POSITION           1000
#define INTAKE_EXTENDED_POSITION            1100
#define INTAKE_OFFSET_POSITION_COUNT          50

// ### Climber Variable ###
#define CLIMBER_DEPLOY_RIGHT_CAN_ID           30
#define CLIMBER_DEPLOY_LEFT_CAN_ID            31
#define CLIMBER_UP_POWER                     0.5
#define CLIMBER_DOWN_POWER                  -0.5

// 180 / Pi
#define MAGIC_NUMBER                    57.29577f
#define ANTIMAGIC_NUMBER                 0.01745f

// ### Command constants ###
// Explanation on commands: 
// Commands are created in OperatorController.cpp as standard methods, written as if they will be popped into another thread
// those commands are then given an ID as below (see C_INTAKE_OB) and in CommandScheduler.cpp it is added in the switch statement
// in OperatorController.cpp the flags are set in the memory shared between the two causing the command to run
#define C_ACTIVE                            0xFF
#define C_KILL                              0x20
#define C_NONE                              0x10
#define C_RUN                               0xF0
#define C_INACTIVE                          0x00

#define C_AMP_OB                            0x01

/// @brief Command scheduler parameter structure
struct CommandParameters
{
    unsigned char command = C_NONE;
    unsigned char state   = C_INACTIVE;
    unsigned char action  = C_NONE;
};
