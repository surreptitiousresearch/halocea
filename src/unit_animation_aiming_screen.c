/* unit_animation_aiming_screen @ 0x836C81E0
   Returns whether the unit's aiming screen is currently active: true when no action
   is playing, but forced false for the airborne/vaulting/landing state band. */
#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

BOOL unit_animation_aiming_screen(unit_animation *animation)
{
    unsigned int state = (unsigned __int8)animation->state;
    BOOL result = animation->action == 0;

    /* forced off from hard_ping through resurrect_back, and for leap_melee */
    if ( state >= _unit_state_hard_ping
      && (state <= _unit_state_resurrect_back || state == _unit_state_leap_melee) )
        return 0;
    return result;
}
