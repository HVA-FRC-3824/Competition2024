#include "../../include/subsystems/Mind.hpp"

Mind::Mind(Turret *orb, Launcher *head, struct TheEye *eye)
{
    /* Load internal references */
    Orb = orb;
    Head = head;
    Eye = eye;

    /* Do other initializers like dashboard and other stuff */
}