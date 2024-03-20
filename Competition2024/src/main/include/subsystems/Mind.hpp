#ifndef THE_MIND_H
#define THE_MIND_H
#include <frc2/command/SubsystemBase.h>
#include "../Constants.h"
#include "Launcher.hpp"

#define HEAD_LOCKED_TO_TAG 0xff
#define HEAD_TRACKING      0x0a
#define READY              0x00
#define SCORE_PLEASE       0xf0

// todo write actuation 
/* The mind accepts info from The Eye, and coordinates our creation's limbs to become omnipresent */
/* Nerd version: coordinates the turret, swerve, launcher, actuation, and whatnots for tags */

class Mind : frc2::SubsystemBase
{
    public:
        Mind(Launcher *head, struct TheEye *eye);
        //void Periodic() override;
        void track_to_tag(int id); /* Tracks then stops, mainly used for testing */
        void lock_to_tag(int id);  /* Keeps tag completely locked, including adaptive tracking */
        void drop();               /* Stops tracking the tag */
        void toggle_lock();        /* Toggles the minds ability */
        bool get_lock();           /* Returns lock status */
        void score();              /* Score based on the currently tracked tag */
        int tracked_tag = 0;
        uint8_t status = READY;
    private:
        void slam_mf_dunk();       /* Internal scoring */
        void adaptive_x(float x);  /* Moves turret based on x coordinates */
        //void adaptive_y(float y);  /* Moves turret basde on y coordinates */
        bool brain_freeze = false;
        bool thinking_direction = 0; /* 1 = left ... 2 = right ... 0 = init / invalid */
        Launcher *Head;
        struct TheEye *Eye;
        /* Height values from tag + up - down, in inches */
        /* NOTE: the distance from the camera to center of launcher will need to be added as an offset*/

        /* Idea for scoring: take the distance from the tag (to camera) and the height from the tag to the goal 
           then do tan(ttg/ttc) (tag to goal and tag to camera respectively) to find the angle offset the actuation needs
        */

        float minds_memory[MAX_TAGS] = {
            /* Blue source */
            0,
            0,
            /* Red Speaker, values untested, lowkey hogwash, ask doc mike about this !!! */
            28,
            28,
            /* Amps */
            -15, 
            -15, 
            /* Blue Speaker, same as above*/
            28,
            28,
            /* Red Source */
            0,
            0,
            /* Stage traps */
            8,
            8,
            8,
            8,
            8,
            8,
        };
};

#endif