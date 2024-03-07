#ifndef AUTOS_H
#define AUTOS_H
#include "../Constants.h"
#include "Swerve.hpp"
#include "Actuation.hpp"
#include <frc/controller/PIDController.h>

#include <unistd.h>

#define A_INACTIVE 0x00
#define A_TRYING_FOR_ANGLE 0x01
#define A_FINISHED 0xff

class Autos
{
    public:
        Autos(Swerve *swerve, cmd_share *commands, Actuation *actuation_obj);
        void do_nothing_auto();
        void shoot_then_move();
        void shoot_then_move_left();
        void shoot_then_move_right();
        void mobility();
        void test_angle();
        unsigned char state = 0;
        frc::PIDController ANGLE_CONTROLLER{ANGLES_SWERVE_P, ANGLES_SWERVE_I, ANGLES_SWERVE_D};
        //burnout();
    private:
        cmd_share* m_commands;
        Swerve* m_swerve;
        Actuation *m_actuation;
};


#endif