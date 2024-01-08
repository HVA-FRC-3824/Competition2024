#ifndef TURRET
#define TURRET
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace ctre::phoenix::motorcontrol::can;



/* Mechanism for turret control */
class Turret : frc2::SubsystemBase
{
    public:
        Turret();
        void Periodic() override;
        void spin_to_angle(int angle);   /* Go to specified angle        */
        void snap_to_swerve();           /* Snaps to heading of swerve   */
        void snap_to_axis(int heading);  /* Locks to n,w,e,s of swerve   */
        void lock_turret();              /* Locks turret position        */
        void spin_simple(float percent); /* Spins based on simple input  */
        int current_heading = 0;
    private:
        WPI_TalonFX TURRET_MOTOR {TURRET_CAN};
        bool locked = false;
};
#endif 

/* Intellecutal contributions from Dr. Oliver Lamour & Dr. Krish Desai */