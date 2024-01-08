#ifndef CONSTANTS
#define CONSTANTS

/* Turret variables */
#define TURRET_GEAR_RATIO 14
#define TURRET_P 0.0
#define TURRET_I 0.0
#define TURRET_D 0.0
#define TURRET_CAN 20
#define TURRET_ROTATIONS_PER_360 TURRET_GEAR_RATIO * TALONFX_UFR

/* Swerve variables */
/* CAN IDS */
#define FR_M 13
#define FL_M 12
#define RL_M 7
#define RR_M 9

#define FR_A 6
#define FL_A 4
#define RL_A 8
#define RR_A 10

/* Hardware Constants */
#define SWERVE_MAX_AMPERAGE 40

/* Swerve Constants */
#define DEADZONE_THRES .25   /* Raise to counter joystick drift this number is on a controller to controller basis */

/* The amount of raw sensor units to complete one full rotation */
#define SWERVE_WHEEL_COUNTS_PER_REVOLUTION 21 /* god neos are weird! */

/* Speed multipliers (rot. speed is a set speed)*/
#define SWERVE_SPEED_MULTIPLIER .5f
#define SWERVE_ROTATION_SPEED .5

/* PID Values for the motorcontrollers */
#define SWERVE_P .1
#define SWERVE_I 0.0
#define SWERVE_D 0.0

/* 180 / Pi */
#define MAGIC_NUMBER 57.29577f

/* Global constants */
#define TALONFX_UFR 2048

#endif