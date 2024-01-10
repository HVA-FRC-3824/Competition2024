#ifndef TURRET_H
#define TURRET_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/InstantCommand.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace ctre::phoenix::motorcontrol::can;

/* Mechanism for turret control */
class Turret : frc2::SubsystemBase
{
    public:
        Turret();
        void Periodic() override;        /* Update our dash and calculate live angle */
        void spin_to_angle(int angle);   /* Go to specified angle                    */
        void snap_to_swerve();           /* Snaps to heading of swerve               */
        void snap_to_axis(int heading);  /* Locks to n,w,e,s of swerve               */
        void lock_turret();              /* Locks turret position                    */
        void spin_simple(float percent); /* Spins based on simple input              */
        void turret_test();              /* Spins based on heading, cycles           */
        frc2::InstantCommand turret_test_command{[this] {turret_test();}, {this}};
        int current_heading = 0;
    private:
        WPI_TalonFX TURRET_MOTOR {TURRET_CAN};
        bool locked = false;
        int test_heading = 0;
};
#endif 

/* Intellecutal contributions from Dr. Oliver Lamour & Dr. Krish Desai */