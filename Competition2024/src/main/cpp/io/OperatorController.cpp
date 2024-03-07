#include "../../include/io/OperatorController.hpp"
#include <iostream>
#include <unistd.h>
#include <frc/smartdashboard/SmartDashboard.h>

void OperatorController::reset_gyro()
{
    //ahrs->Reset();
    std::cout << "Gyro Reset\n";
}

void OperatorController::one_button_intake()
{
    std::cout << "command start\n";
    state = O_SOFT_LOCK; /* Ignore user input until command executes */

    /* Transfer of note into index */
    m_launcher->index_spin(.5);
    m_intake->suck(.3);

    usleep(500 * 1000);
    
    /* Return intake */

    m_intake->suck(0);
    m_launcher->index_spin(0);

    //usleep(500 * 1000);

    /* Back out note in index */
    m_launcher->index_spin(-.1);

    usleep(150 * 1000);

    m_launcher->index_spin(0);

    usleep(200 * 1000);

    m_actuation->locked = false;
    state = O_ACTIVE; 
}

void OperatorController::one_button_shoot()
{
    state = O_SOFT_LOCK;
    
    /* Rev up launcher */
    m_launcher->simple_spin(1);

    /* Wait 1 seconds */
    usleep(1000 * 1000);

    m_launcher->index_spin(1);

    usleep(250 * 1000);

    m_launcher->simple_spin(0);
    m_launcher->index_spin(0);
    state = O_ACTIVE;
}

void OperatorController::one_button_amp()
{
    state = O_SOFT_LOCK;
    m_launcher->simple_spin(.1);
    m_launcher->index_spin(.25);
    usleep(500 * 1000);
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
        if(state == O_SOFT_LOCK){frc::SmartDashboard::PutBoolean("Softlocked? ", true); return; /* computer only! */}
        frc::SmartDashboard::PutBoolean("Softlocked? ", false);

        //if(OperatorStick.GetRawButtonPressed(5)){if(mode == A_MODE){mode = B_MODE;}else{mode = A_MODE;};}

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
                double z; 
                z = OperatorStick.GetZ();
                double y; 
                y = -OperatorStick.GetY();
                int index; 
                index = OperatorStick.GetRawAxis(3);

                if(z < .2 && z > -.2){z = 0;}
                if(y < .75 && y > -.75){y = 0;} 

                m_turret->spin_simple(z * .2);
                m_actuation->linear_actuation(y);
                m_launcher->index_spin(index * .25); 
                //m_launcher->simple_spin(y);


                if(OperatorStick.GetPOV(0) == 0)
                {
                    m_intake->intake_actuate_simple(-1);
                } else if(OperatorStick.GetPOV(0) == 180)
                {
                    m_intake->intake_actuate_simple(1);
                } else {
                    m_intake->intake_actuate_simple(0);
                }

                break;
            case B_MODE:
                break;
        }

        /* THUMB */
        if(OperatorStick.GetRawButtonPressed(2))
        {
            this->shared->command_being_run = C_AMP_OB;
            this->shared->my_wishes = C_RUN; 
        }

        /* TOP_3 */
        if(OperatorStick.GetRawButtonPressed(5))
        {
            switch(mode)
            {
                case A_MODE:
                    this->m_intake->suck(1);
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

        /* TOP_5 */
        if(OperatorStick.GetRawButtonPressed(3))
        {
            switch(mode)
            {
                case A_MODE:
                    this->m_intake->suck(0);
                    break;
                case B_MODE:
                    break;
                case O_TEST:
                    this->m_turret->spin_to_angle(0);
                    reset_gyro();
                    break;
            }
        }

        if(OperatorStick.GetRawButtonPressed(6))
        {
            one_button_intake();
        }

        /* TOP_4 */
        if(OperatorStick.GetRawButtonPressed(4))
        {
            switch(mode)
            {
            case A_MODE:
                this->m_intake->suck(-1);
                break;
            case B_MODE:
                this->m_actuation->actuate_to_point(0);
                break;
            case O_TEST:
                break;      
            }     
        }

        /* TRIGGER*/
        if(OperatorStick.GetRawButtonPressed(1))
        {
            this->shared->command_being_run = C_LAUNCHER_OB;
            this->shared->my_wishes = C_RUN; 
        }
    }
}

OperatorController::OperatorController(cmd_share *share, Turret *turret_obj, Launcher *launcher_obj, Intake *intake_obj, Actuation *actuation_obj, Climb *climb_obj)
{
    m_turret = turret_obj;
    //ahrs = ahrs_obj;
    m_launcher = launcher_obj;
    m_intake = intake_obj;
    m_actuation = actuation_obj;
    //+m_climb = climb_obj;
    shared = share;
}