#include "../../include/io/OperatorController.hpp"
#include <iostream>
#include <unistd.h>
#include <frc/smartdashboard/SmartDashboard.h>

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

    usleep(250*1000);

    m_actuation->locked = true;

    /* Check if turret is zeroed, if not zero it */
    while(!((m_turret->current_heading >= 0-TURRET_MARGIN_OF_ERROR)&&(m_turret->current_heading <= 0+TURRET_MARGIN_OF_ERROR)))
    {
        m_turret->spin_to_angle(0); /* snaps forward */
    }

    m_turret->lock_turret();

    /* Once turret is zeroed, and launcher in correct position, fold intake in */
    if(m_intake->retracted == false)
    {
        m_intake->flip_retraction();
        sleep(1);
    }

    /* Transfer of note into index */
    m_launcher->index_spin(-.5);
    m_actuation->actuate_to_point(18);
    m_intake->suck(-.3);

    usleep(1500 * 1000);
    
    /* Return intake */

    m_actuation->actuate_to_point(ACTUATION_INDEX_ANGLE);
    m_intake->suck(0);
    m_launcher->index_spin(0);

    usleep(500 * 1000);

    /* Back out note in index */
    m_launcher->index_spin(.1);

    usleep(150 * 1000);

    m_launcher->index_spin(0);

    m_intake->flip_retraction();

    usleep(200 * 1000);

    m_turret->lock_turret(); // unlocks, toggle
    m_actuation->locked = false;
    state = O_ACTIVE; 
}

void OperatorController::one_button_shoot()
{
    state = O_SOFT_LOCK;
    
    /* Rev up launcher */
    m_launcher->simple_spin(-1);

    /* Wait 1.5 seconds */
    usleep(1500 * 1000);

    m_launcher->index_spin(-1);

    usleep(250 * 1000);

    m_launcher->simple_spin(0);
    m_launcher->index_spin(0);
    state = O_ACTIVE;
}

void OperatorController::robo_periodic()
{
    //std::cout << "OPERATOR\n";
    //m_intake->suck(OperatorStick.GetRawAxis(3));
    //if(OperatorStick.GetRawButton(1)){std::cout << "BUTTON!!\n";}

    /* TODO: add driverstation updates for controller mode */

    if(state)
    {
        if(state == O_SOFT_LOCK){frc::SmartDashboard::PutBoolean("Softlocked? ", false); return; /* computer only! */}
        frc::SmartDashboard::PutBoolean("Softlocked? ", true);

        if(OperatorStick.GetRawButtonPressed(5)){if(mode == A_MODE){mode = B_MODE;}else{mode = A_MODE;};}

        switch(mode)
        {
            case A_MODE:
                frc::SmartDashboard::PutString("Operator Mode: ", "A_MODE");
                break;
            case B_MODE:
                frc::SmartDashboard::PutString("Operator Mode: ", "B_MODE");
                break;
        }



        /* This is a ridiculously bad way to do this :D */
        /* Return always after a hit condition to save resources */
        /* This is done below to prevent sticky values, since GetRawButtonPressed gets the value since it was last called
           not firing the method since (ie. until a new mode is selected) will cause the old value to be used
           better solution is to poll ALL buttons at once and then do actions based on the mode instead of vice versa */

        switch(mode)
        {
            case A_MODE:
                /*m_launcher->index_spin(OperatorStick.GetRawAxis(5));
                m_launcher->simple_spin(OperatorStick.GetY()); */
                m_actuation->linear_actuation(-OperatorStick.GetRawAxis(5));
                m_intake->suck(OperatorStick.GetRawAxis(2)-OperatorStick.GetRawAxis(3)); 
                m_launcher->index_spin(OperatorStick.GetRawAxis(5));
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
                    this->m_actuation->actuate_to_point(-20);
                    break;
                case B_MODE:
                    std::cout << "Operator Called\n";
                    this->shared->command_being_run = C_INTAKE_OB;
                    this->shared->my_wishes = C_RUN; 
                    break;
                case O_TEST:
                    this->m_turret->spin_to_angle(90);
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
                    this->shared->command_being_run = C_LAUNCHER_OB;
                    this->shared->my_wishes = C_RUN;
                    //m_intake->flip_retraction();
                    //m_actuation->actuate_to_point(0);
                    //this->m_intake->intake_actuate_point(INTAKE_BOTTOM_POINT);
                    break;
                case B_MODE:
                    break;
                case O_TEST:
                    this->m_turret->spin_to_angle(0);
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