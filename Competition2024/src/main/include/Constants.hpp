#pragma once

// CAN bus name (specifically for TalonFX)
#define CANBUS_NAME                          "rio"

// Swerve CAN IDs Driver Motors
#define FR_M_CAN_ID                              7
#define FL_M_CAN_ID                             12
#define RL_M_CAN_ID                             13
#define RR_M_CAN_ID                             10

// Swerve CAN IDs Angle Motors
#define FR_A_CAN_ID                              8
#define FL_A_CAN_ID                              4
#define RL_A_CAN_ID                              6
#define RR_A_CAN_ID                              9

// Swerve CAN IDs Encoders
#define FR_E_CAN_ID                             41
#define FL_E_CAN_ID                             43
#define RL_E_CAN_ID                             40
#define RR_E_CAN_ID                             42

#define INTAKE_ACTUATION_CAN_ID                 20
#define INTAKE_ROLLER_MOTOR_CAN_ID              21

#define CLIMBER_RIGHT_CAN_ID                    30
#define CLIMBER_LEFT_CAN_ID                     31

// 180 / Pi
#define MAGIC_NUMBER                      57.29577f
#define ANTIMAGIC_NUMBER                   0.01745f
 
// ### Controller constants ###
#define DRIVER_CONTROLLER                        0
#define OPERATOR_CONTROLLER                      1

#define JOYSTICK_BUTTON_A                        1
#define JOYSTICK_BUTTON_B                        2
#define JOYSTICK_BUTTON_X                        3
#define JOYSTICK_BUTTON_Y                        4

#define JOYSTICK_BUMPER_LEFT                     5
#define JOYSTICK_BUMPER_RIGHT                    6

#define JOYSTICK_POV_0                           0
#define JOYSTICK_POV_90                         90
#define JOYSTICK_POV_180                       180
#define JOYSTICK_POV_270                       270

#define LEFT_TRIGGER_AXIS                         2
#define RIGHT_TRIGGER_AXIS                        3
#define TRIGGER_THRESHOLD_VALUE                 0.5

// #### Swerve ####
#define CHASSIS_LENGTH                          30
#define CHASSIS_WIDTH                           30

#define SWERVE_MODULES                           4

// Hardware Constants
#define SWERVE_MAX_AMPERAGE                     28

// Swerve Constants
#define DEADZONE_THRESHOLD                     0.1   // Raise to counter joystick drift this number is on a controller to controller basis

// The amount of raw sensor units to complete one full rotation
#define SWERVE_WHEEL_COUNTS_PER_REVOLUTION      21

// PID Values for the motorcontrollers
#define SWERVE_P                             0.700
#define SWERVE_I                             0.000
#define SWERVE_D                             0.001

// PID Values for angle controller
#define ANGLES_SWERVE_P                       0.05
#define ANGLES_SWERVE_I                       0.00
#define ANGLES_SWERVE_D                       0.00

// ### Intake variables ###
#define INTAKE_ROLLER_MOTOR_JOYSTICK_DEADBAND  0.1
#define INTAKE_MAXIMUM_ROLLER_POWER            0.5  // MAximum power to the roller motor

#define INTAKE_ACTUATION_SENSOR_RATIO          1.0  // Adjust per robot (could set to 1 and just use encoder counts)

#define INTAKE_ACTUATION_A                   0.000  // Adceleration FeedForward
#define INTAKE_ACTUATION_D                   0.000  // Derivative Gain
#define INTAKE_ACTUATION_G                   0.000  // Gravity FeedForward Gain
#define INTAKE_ACTUATION_I                   1.000  // Integra Gain
#define INTAKE_ACTUATION_P                  25.000  // Proportional Gain
#define INTAKE_ACTUATION_S                   0.000  // Static FeedForward Gain
#define INTAKE_ACTUATION_V                   1.000  // Velocity FeedFroward Gain

#define INTAKE_ACTUATION_ACCELERATION      150.000  // Move Acceleration       - INCREASE FOR FASTER ACCELERATION
#define INTAKE_ACTUATION_VELOCITY          100.000  // Maximum Move Velocity   - SETS MAXIMUM SPEED
#define INTAKE_ACTUATION_JERK              200.000  // Acceleration Derivative - INCREASE FOR MORE SPEED (200 - 500)

#define INTAKE_START_POSITION                0.000  // Should always start at zero
#define INTAKE_AMP_POSITION                  5.700  // Adjust to the desired Amp position
#define INTAKE_FEED_POSITION                 8.500  // Adjust to the desired Feed position
#define INTAKE_CLIMB_POSITION               12.000 // Adust to be out of the way for climb.
#define INTAKE_POSITION_REACHED_POSITION     0.500  // Indicates the position offset to change intake position (Amp or Feed)

#define INTAKE_POSITION_STEP_VALUE           0.100  // Sets the "jog" step size

#define INTAKE_MAX_AMPERAGE                     30  // The maximum intake motor amperage

// ### Climber Variable ###
#define CLIMBER_MOTORS                           2

#define CLIMBER_MOTOR_RIGHT                      0
#define CLIMBER_MOTOR_LEFT                       1

#define CLIMB_MAXIMUM_OUTPUT_CURRENT            60  // Threshold to disable the climb motors (reached full climb)

#define CLIMBER_UP_POWER                       1.0
#define CLIMBER_DOWN_POWER                    -1.0

// ### Autonomous Variables ###
// Autonomous Move
#define AUTO_DRIVE_FORWARD_SPEED              -0.4
#define AUTO_DRIVE_FORWARD_TIME        2000 * 1000  // Two seconds

// Autonomous place AMP and move
#define AUTO_SCOOT_TO_AMP_SPEED                0.2
#define AUTO_SCOOT_TO_AMP_TIME          800 * 1000

#define AUTO_DRIVE_TO_AMP_SPEED                0.2
#define AUTO_DRIVE_TO_AMP_TIME         1200 * 1000

#define AUTO_PLACE_NOTE                        1.0
#define AUTO_PLACE_NOTE_TIME           1200 * 1000

#define AUTO_DRIVE_ACROSS_LINE_SPEED           0.2
#define AUTO_DRIVE_ACROSS_LINE_TIME    1000 * 1000
