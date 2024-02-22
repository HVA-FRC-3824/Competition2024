#ifndef DRIVER_CONTROLLER_H
#define DRIVRE_CONTROLLER_H
#include "../Constants.h"
#include "../subsystems/Swerve.hpp"
#include <frc/Joystick.h>

class DriverController
{
    public:
        DriverController(Swerve *swerve_obj);
        void robo_periodic();
    private:
        Swerve *m_swerve;
        frc::Joystick DriverStick{DRIVER_CONTROLLER};
        bool field_centric = false;
};

#endif