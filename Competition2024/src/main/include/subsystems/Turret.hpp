#ifndef TURRET_H
#define TURRET_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/InstantCommand.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <ctre/phoenix6/TalonFX.hpp>
#include "../Memory.h"

using namespace ctre::phoenix::motorcontrol::can;

/* Mechanism for turret control */
class Turret : frc2::SubsystemBase
{
    public:
        Turret(angle_mem_share *share);
        void robo_periodic();            /* Update our dash and calculate live angle */
        void spin_to_angle(float angle);   /* Go to specified angle                    */
        void snap_to_swerve();           /* Snaps to heading of chassis              */
        void snap_to_axis(int heading);  /* Locks to n,w,e,s of chassis              */
        void lock_turret();              /* Locks turret position                    */
        void soft_lock_turret();         /* Soft locks turret, see below for def.    */
        void spin_simple(float percent); /* Spins based on simple input              */
        void turret_test();              /* Spins based on heading, cycles           */
        frc2::InstantCommand turret_test_command{[this] {turret_test();}, {this}};
        int current_heading = 0;
    private:
        ctre::phoenix6::hardware::TalonFX TURRET_MOTOR {TURRET_CAN,"rio"};
        bool locked = false;      /* Turret CANNOT MOVE WHATSOEVER, NO MATTER WHAT! */
        bool soft_locked = false; /* Operator CANNOT MOVE IT, computer TAKES OVER   */
        /* Note on soft locked, it is ONLY CHECK in the operator controller class, soft lock should not need to be
           checked internally, this only slows down the computer controls */
        int test_heading = 0;
        angle_mem_share *internal_reference;
};
#endif 

/* Intellecutal contributions from Dr. Oliver Lamour & Dr. Krish Desai */