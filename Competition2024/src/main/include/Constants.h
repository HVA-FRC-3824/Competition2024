#ifndef CONSTANTS
#define CONSTANTS

/* Not on CAN Ids and naming, anything above or on the turret should be prefixed 2#, such as 20 for the physical turret */
/* IDs should be numbered starting from the turret up, incrementing each time                                           */
/* Anything below should be 1# or 0# */

/* Turret variables */
#define TURRET_GEAR_RATIO 14
#define TURRET_P 0.1
#define TURRET_I 0.0
#define TURRET_D 0.0
#define TURRET_CAN 20
#define TURRET_MARGIN_OF_ERROR 2 /* +- degrees */
#define TURRET_ROTATIONS_PER_360 TURRET_GEAR_RATIO * TALONFX_UFR

/* Launcher variables */
#define LEFT_MOTOR_CAN 21
#define RIGHT_MOTOR_CAN 22
#define INDEX_MOTOR_CAN 23

/* NOTE: both motors will share the same PIDs 
PID values for launch motors SUBJECT TO CHANGE*/
#define LAUNCH_MOTOR_P 0.01
#define LAUNCH_MOTOR_I 0.0
#define LAUNCH_MOTOR_D 0.0

/* THE EYE VARIABLES */
#define THE_EYE_PORT 5805
#define MAX_TAGS 16  /* Note: this is also the max viewable at one time. */
#define MATCH_TAGS MAX_TAGS / 2 
#define THE_EYE_TIMEOUT .25 /* Seconds */
#define EYE_X 640
#define EYE_Y 480


/* THE MIND VARIABLES */
#define T3_CORRECTION_HOR (5.0/360) * (TURRET_ROTATIONS_PER_360)
#define T2_CORRECTION_HOR (2.0/360) * (TURRET_ROTATIONS_PER_360)
#define T1_CORRECTION_HOR (.5/360) * (TURRET_ROTATIONS_PER_360)
#define X_ACCEPTABLE_RANGE 25 /* 25 pixel diff, may need to be adjusted based on distance! */
// tbd: need to test actuation and actual angle
#define T3_CORRECTION_VERT (5.0/360) * (TURRET_ROTATIONS_PER_360)
#define T2_CORRECTION_VERT (5.0/360) * (TURRET_ROTATIONS_PER_360)
#define T1_CORRECTION_VERT (5.0/360) * (TURRET_ROTATIONS_PER_360)

/* Intake variables */
#define INTAKE_MOTOR 16
#define INTAKE_ACTUATION 17
#define INTAKE_ACTUATION_P .005
#define INTAKE_ACTUATION_I 0.0
#define INTAKE_ACTUATION_D 0.0
#define INTAKE_ACTUATION_UFR TALONFX_UFR * (24/18)
#define INTAKE_BOTTOM_POINT 40835

/* Actuation variables */
#define ACTUATION_ID 24
#define ACTUATION_GR 183.5 /*I believe??*/
#define ACTUATION_P 0.005
#define ACTUATION_I 0
#define ACTUATION_D 0 
#define ACTUATION_INDEX_ANGLE 0

/* Swerve variables */
/* CAN IDS */
#define FR_M 7
#define FL_M 12
#define RL_M 13
#define RR_M 10

#define FR_A 8
#define FL_A 4
#define RL_A 6
#define RR_A 9

/* Hardware Constants */
#define SWERVE_MAX_AMPERAGE 30

/* Swerve Constants */
#define DEADZONE_THRES .25   /* Raise to counter joystick drift this number is on a controller to controller basis */

/* The amount of raw sensor units to complete one full rotation */
#define SWERVE_WHEEL_COUNTS_PER_REVOLUTION 21 /* god neos are weird! */

/* Speed multipliers (rot. speed is a set speed)*/
#define SWERVE_SPEED_MULTIPLIER 1
#define SWERVE_ROTATION_SPEED .5

/* PID Values for the motorcontrollers */
#define SWERVE_P .7
#define SWERVE_I 0.0
#define SWERVE_D 0.001

/* 180 / Pi */
#define MAGIC_NUMBER 57.29577f

/* Global constants */
#define TALONFX_UFR 2048
#define OPERATOR_CONTROLLER 0
#define DRIVER_CONTROLLER 1

/* Command constants */

/* Explanation on commands: 
   commands are created in OperatorController.cpp as standard methods, written as if they will be popped into another thread
   those commands are then given an ID as below (see C_INTAKE_OB) and in CommandHelper.cpp it is added in the switch statement
   in OperatorController.cpp the flags are set in the memory shared between the two causing the command to run.
*/

#define C_ACTIVE    0xff
#define C_NONE      0x10
#define C_RUN       0xf0
#define C_INACTIVE  0xf1
#define C_DISABLED  0x00
#define C_INTAKE_OB 0x01

struct CommandMemoryShare
{
    unsigned char command_being_run = C_NONE;
    unsigned char state = C_INACTIVE;
    unsigned char my_wishes = C_NONE;
};
typedef CommandMemoryShare cmd_share;

#endif
