#include "../../include/io/OperatorController.hpp"
#include <iostream>
#include <unistd.h>

void OperatorController::reset_gyro()
{
    ahrs->Reset();
    std::cout << "Gyro Reset\n";
}

void OperatorController::one_button_intake()
{
    std::cout << "command start\n";
    state = O_SOFT_LOCK; /* Ignore user input until command executes */
    
    /* Make sure angle of launcher is in correct position */
    m_actuation->actuate_to_point(ACTUATION_INDEX_ANGLE);

    sleep(.25);

    m_actuation->locked = true;

    /* Check if turret is zeroed, if not zero it */
    while(!((m_turret->current_heading >= 0-TURRET_MARGIN_OF_ERROR)&&(m_turret->current_heading <= 0+TURRET_MARGIN_OF_ERROR)))
    {
        m_turret->snap_to_axis(0); /* snaps forward */
    }

    m_turret->lock_turret();

    /* Once turret is zeroed, and launcher in correct position, fold intake in */
    m_intake->flip_retraction();

    sleep(.25);

    /* Transfer of note into index */
    m_launcher->index_spin(.5);
    m_intake->suck(-.3);

    sleep(3);
    
    /* Return intake */
    m_intake->suck(0);
    m_launcher->index_spin(0);
    m_intake->flip_retraction();

    sleep(.2);

    /* Back out note in index */
    m_launcher->index_spin(-.5);

    sleep(1);

    m_launcher->index_spin(0);

    m_turret->lock_turret();
    m_actuation->locked = false;
    state = O_ACTIVE;
}

void OperatorController::robo_periodic()
{
    //std::cout << "OPERATOR\n";
    m_intake->suck(OperatorStick.GetRawAxis(3));
    //if(OperatorStick.GetRawButton(1)){std::cout << "BUTTON!!\n";}

    /* TODO: add driverstation updates for controller mode */

    if(state)
    {
        if(state == O_SOFT_LOCK){ return; /* computer only! */}
        if(OperatorStick.GetRawButtonPressed(5)){if(mode == O_TEST){mode = A_MODE;}else{mode = O_TEST;};}

        /* This is a ridiculously bad way to do this :D */
        /* Return always after a hit condition to save resources */
        /* This is done below to prevent sticky values, since GetRawButtonPressed gets the value since it was last called
           not firing the method since (ie. until a new mode is selected) will cause the old value to be used
           better solution is to poll ALL buttons at once and then do actions based on the mode instead of vice versa */

        switch(mode)
        {
            case A_MODE:
                /*m_launcher->index_spin(OperatorStick.GetRawAxis(5));
                m_launcher->simple_spin(OperatorStick.GetY());
                m_intake->suck(OperatorStick.GetRawAxis(2)-OperatorStick.GetRawAxis(3)); */
                break;
            case B_MODE:
                m_actuation->linear_actuation(OperatorStick.GetY() * .15);
                m_intake->intake_actuate_simple(OperatorStick.GetRawAxis(5) * .15);
                break;
        }

        /* A */
        if(OperatorStick.GetRawButtonPressed(1))
        {
            switch(mode)
            {
                case A_MODE:
                    this->shared->command_being_run = C_INTAKE_OB;
                    this->shared->my_wishes = C_RUN;
                    break;
                case B_MODE:
                    break;
                case O_TEST:
                    this->m_turret->turret_test();
                    return;
                    break;
            }        
        }

        /* X */
        if(OperatorStick.GetRawButtonPressed(3))
        {
            switch(mode)
            {
                case A_MODE:
                    //this->m_intake->intake_actuate_point(INTAKE_BOTTOM_POINT);
                    break;
                case B_MODE:
                    break;
                case O_TEST:
                    reset_gyro();
                    break;
            }
        }
    }
}

OperatorController::OperatorController(cmd_share *share, Turret *turret_obj, AHRS *ahrs_obj, Launcher *launcher_obj, Intake *intake_obj, Actuation *actuation_obj)
{
    m_turret = turret_obj;
    ahrs = ahrs_obj;
    m_launcher = launcher_obj;
    m_intake = intake_obj;
    m_actuation = actuation_obj;
    shared = share;
}