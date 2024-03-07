#include "../../include/subsystems/Autos.hpp"
#include "../../include/subsystems/Actuation.hpp"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

/* Note on commands in general: each command runs in a thread independant of this current thread
   so if you NEED to wait for a command to complete then you must wait in this thread
*/

/* IMPORTANT! Make sure ALL sleeps and delays do not total over 15 seconds for one auto */

Autos::Autos(Swerve *swerve, cmd_share *commands, Actuation *actuation_obj)
{
    m_swerve = swerve;
    m_commands = commands;
    m_actuation = actuation_obj;
}

void Autos::do_nothing_auto()
{
    frc::SmartDashboard::PutString("AutoName: ", "LOSER AUTO!!!!");
    usleep(15000 * 1000); /* Sleep for 15 seconds */
}

void Autos::shoot_then_move_left()
{
    m_commands->command_being_run = C_LAUNCHER_OB;
    m_commands->my_wishes = C_RUN; 

    usleep(1000 * 1000);
    m_swerve->drive(0,-.2,0,0);

    usleep(1000 * 1000);
    m_swerve->drive(-.4,0,0,0);
    usleep(2000 * 1000);
    m_swerve->drive(0,0,0,0);
}

void Autos::shoot_then_move_right()
{
    m_commands->command_being_run = C_LAUNCHER_OB;
    m_commands->my_wishes = C_RUN; 

    usleep(1000 * 1000);
    m_swerve->drive(0,.2,0,0);

    usleep(1500 * 1000);
    m_swerve->drive(-.4,0,0,0);
    usleep(2000 * 1000);
    m_swerve->drive(0,0,0,0);
}

void Autos::mobility()
{
    m_swerve->drive(-.4,0,0,0);
    usleep(2000 * 1000);
    m_swerve->drive(0,0,0,0);
}

void Autos::shoot_then_move()
{
    /* lower us */
    /*m_actuation->linear_actuation(-1);
    usleep(250 * 1000);
    m_actuation->linear_actuation(0);*/

    frc::SmartDashboard::PutString("AutoName: ", "Just Shoot");

    /* Schedule launch command */
    m_commands->command_being_run = C_LAUNCHER_OB;
    m_commands->my_wishes = C_RUN; 

    /* Sleep for command duration */
    //usleep(1250 * 1000);

    /* Drive */
    //mobility();
}

void Autos::test_angle()
{
    this->state = A_TRYING_FOR_ANGLE;
    this->ANGLE_CONTROLLER.SetSetpoint(60);
    while(!this->ANGLE_CONTROLLER.AtSetpoint())
    {}
}