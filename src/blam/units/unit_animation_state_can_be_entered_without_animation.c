/* unit_animation_state_can_be_entered_without_animation @ 0x836C8278
   Returns whether a given animation state may be entered without a real animation. The melee and
   leap attack states (melee_attack/airborne/continuous, throw_grenade, leap_start, leap_melee)
   require a real animation; every other state may be entered without one.

   Deviation: the decompiler rendered this as switch(new_state - 30) with the offset cases
   0,1,2,3,9,11 (a jump table based at state 30); reproduced here as an equivalent switch over the
   named states. */

#include <stdint.h>
#include "headers/unit_animation_state.h"

int unit_animation_state_can_be_entered_without_animation(int16_t new_state)
{
    switch ( new_state )
    {
        case _unit_state_melee_attack:      /* 30 */
        case _unit_state_melee_airborne:    /* 31 */
        case _unit_state_melee_continuous:  /* 32 */
        case _unit_state_throw_grenade:     /* 33 */
        case _unit_state_leap_start:        /* 39 */
        case _unit_state_leap_melee:        /* 41 */
            return 0;
        default:
            return 1;
    }
}
