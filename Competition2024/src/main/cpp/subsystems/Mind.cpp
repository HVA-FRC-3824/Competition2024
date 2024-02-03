#include "../../include/subsystems/Mind.hpp"

Mind::Mind(Turret *orb, Launcher *head, struct TheEye *eye)
{
    /* Load internal references */
    Orb = orb;
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
    switch(tier)
    {
        case 1:
            if(direction == 1)
            {
                Orb->spin_to_angle(Orb->current_heading-T1_CORRECTION_HOR);
            } else {
                Orb->spin_to_angle(Orb->current_heading+T1_CORRECTION_HOR);
            }
            break;
        case 2:
            if(direction == 1)
            {
                Orb->spin_to_angle(Orb->current_heading-T2_CORRECTION_HOR);
            } else {
                Orb->spin_to_angle(Orb->current_heading+T2_CORRECTION_HOR);
            }
            break;
        case 3:
            if(direction == 1)
            {
                Orb->spin_to_angle(Orb->current_heading-T3_CORRECTION_HOR);
            } else {
                Orb->spin_to_angle(Orb->current_heading+T3_CORRECTION_HOR);
            }
            break;
        default:
            return;
    }
}


void Mind::track_to_tag(int id)
{
    if(brain_freeze){return;}

    tracked_tag = id;
    status = HEAD_TRACKING;
    if(Eye->TheEyes_Subjects[id-1].tag_status == ACTIVE)
    {
        adaptive_x(Eye->TheEyes_Subjects[id-1].tag.center_x);
    } else if (Eye->TheEyes_Subjects[id-1].tag_status == INACTIVE)
    {
        if(thinking_direction == 1)
        {
            Orb->spin_to_angle(Orb->current_heading-T2_CORRECTION_HOR);
        } else if (thinking_direction == 2)
        {
            Orb->spin_to_angle(Orb->current_heading+T2_CORRECTION_HOR);
        }
        
    }
    tracked_tag = 0;
    status = READY;
}

void Mind::lock_to_tag(int id)
{
    tracked_tag = id;
    if(status != HEAD_LOCKED_TO_TAG){status = HEAD_LOCKED_TO_TAG;}
    while(HEAD_LOCKED_TO_TAG)
    {
        track_to_tag(id);
    }
}

void Mind::drop()
{
    status = READY; /* breaks while loop above and readies mind */
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