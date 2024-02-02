#ifndef THE_MIND_H
#define THE_MIND_H
#include <frc2/command/SubsystemBase.h>
#include "../Constants.h"
#include "Turret.hpp"
#include "Launcher.hpp"
#include "TheEye.h"

#define HEAD_LOCKED_TO_TAG 0xff
#define HEAD_TRACKING      0x0a
#define HEAD_ADAPTIVE_ON   0x0b
#define READY              0x00

// todo write actuation 
/* The mind accepts info from The Eye, and coordinates our creation's limbs to become omnipresent */
/* Nerd version: coordinates the turret, swerve, launcher, actuation, and whatnots for tags */

class Mind : frc2::SubsystemBase
{
    public:
        Mind(Turret *orb, Launcher *head, struct TheEye *eye);
        void Periodic() override;
        void track_to_tag(int id); /* Tracks then stops, mainly used for testing */
        void lock_to_tag(int id);  /* Keeps tag completely locked, including adaptive tracking */
        void drop();               /* Stops tracking the tag */
        void toggle_lock();        /* Toggles the minds ability */
        bool get_lock();           /* Returns lock status */
        int tracked_tag = 0;
        uint8_t status = READY;
    private:
        bool brain_freeze = false;
        Turret *Orb;
        Launcher *Head;
        struct TheEye *Eye;
};

#endif