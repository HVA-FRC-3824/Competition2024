#include "../../include/subsystems/Mind.hpp"

Mind::Mind(Launcher *head, struct TheEye *eye)
{
    /* Load internal references */
    Head = head;
    Eye = eye;

    /* Do other initializers like dashboard and other stuff */
}

void Mind::adaptive_x(float x)
{
    uint8_t tier = 0;
    uint8_t direction = 0;

    /* Do a check if it's already centered, may need to adjust these values as necessary */

    if(x >= EYE_X - X_ACCEPTABLE_RANGE && x <= EYE_X + X_ACCEPTABLE_RANGE){return;}

    if(x < (EYE_X / 2)) { direction = 1;} else { direction = 2;}
    thinking_direction = direction; /* Last direction it went, in the case of a lost tag it will move that direction slowly */

    /* Sort x into a tier */
    /* hope this lowkey autism magic works out?? we'll see!! */
    if(x >= (EYE_X * .65) || x <= (EYE_X * .35))
    {
        tier = 3;
    } else if (!(tier) && (x >= (EYE_X * .535) || x <= (EYE_X * .475)))
    {
        tier = 2;
    } else if (tier != 3 && tier != 2)
    {
        tier = 1;
    }

    /* Warning! This MAY need to be changed in the future! Turret SHOULD only go 0-360... FUTURE ZANE: FIX THIS!! */
    /* Or, better idea, just keep it negatives, say fuck it, live life! */
    
}


void Mind::track_to_tag(int id)
{
    if(brain_freeze){return;} 

    tracked_tag = id;
    if(Eye->TheEyes_Subjects[id-1].tag_status == ACTIVE)
    {
        adaptive_x(Eye->TheEyes_Subjects[id-1].tag.center_x);
    } else if (Eye->TheEyes_Subjects[id-1].tag_status == INACTIVE)
    {
        /* Write code for loss of tag, atm it just stops/nothing happens */
        /*
        if(thinking_direction == 1)
        {
            Orb->spin_to_angle(Orb->current_heading-T2_CORRECTION_HOR);
        } else if (thinking_direction == 2)
        {
            Orb->spin_to_angle(Orb->current_heading+T2_CORRECTION_HOR);
        }*/
        
    }
    tracked_tag = 0;
}

void Mind::slam_mf_dunk()
{
    /* see Mind.hpp for idea behind the mind's main circuit!! */
    /* float adjustment_angle;
    float final_angle; */



}

void Mind::score()
{
    /* Flags the minds loop so it can score while remaining locked */
    if(status == HEAD_LOCKED_TO_TAG){status = SCORE_PLEASE; return;};
    slam_mf_dunk();
}

/* NOTE: this MUST be popped out in a thread... for this while loopy !! */
void Mind::lock_to_tag(int id)
{
    tracked_tag = id;
    if(status != HEAD_LOCKED_TO_TAG){status = HEAD_LOCKED_TO_TAG;}
    while(HEAD_LOCKED_TO_TAG)
    {
        if(status == SCORE_PLEASE){slam_mf_dunk(); status = HEAD_LOCKED_TO_TAG; continue;}
        track_to_tag(id);
    }
}

void Mind::drop()
{
    status = READY; /* breaks while loop above and readies the mind */
    tracked_tag = 0;
    thinking_direction = 0;
}

void Mind::toggle_lock()
{
    brain_freeze = !brain_freeze;
}

bool Mind::get_lock()
{
    return brain_freeze;
}