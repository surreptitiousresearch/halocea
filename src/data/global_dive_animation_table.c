/* global_dive_animation_table @ 0x821289D8 — .rdata, 56 bytes = 7 x dive_animation_possibility
 * (sizeof == 8; 56 / 8 == 7, no padding). actor_action_try_to_dive walks it, scoring each row as
 * direction_scores[animation_direction] + weight and keeping the best playable impulse; the walk
 * ends at the row whose animation_impulse is -1. The dive_* impulses carry a +1.5 bias so they
 * are preferred over the evade_* impulses for the same direction.
 * Raw big-endian words -> resolved meaning (impulse | direction, weight):
 *   [0] 0x000A0000 0x3FC00000 -> _unit_animation_impulse_dive_left,   _actor_evade_left,    1.5
 *   [1] 0x00060000 0x00000000 -> _unit_animation_impulse_evade_left,  _actor_evade_left,    0.0
 *   [2] 0x000B0001 0x3FC00000 -> _unit_animation_impulse_dive_right,  _actor_evade_right,   1.5
 *   [3] 0x00070001 0x00000000 -> _unit_animation_impulse_evade_right, _actor_evade_right,   0.0
 *   [4] 0x00080002 0x3FC00000 -> _unit_animation_impulse_dive_front,  _actor_evade_forward, 1.5
 *   [5] 0x00090003 0x3FC00000 -> _unit_animation_impulse_dive_back,   _actor_evade_back,    1.5
 *   [6] 0xFFFFFFFF 0x00000000 -> terminator (-1)
 *
 * The object is in .rdata; the corpus declaration in src/actor_action_try_to_dive.c is non-const
 * and the definition matches it so every declaration of the symbol agrees.
 */

#include <stdint.h>
#include "../headers/dive_animation_possibility.h"
#include "../headers/unit_animation_impulse.h"
#include "../headers/actor_evade.h"

dive_animation_possibility global_dive_animation_table[7] =
{
    { _unit_animation_impulse_dive_left,   _actor_evade_left,    1.5f },
    { _unit_animation_impulse_evade_left,  _actor_evade_left,    0.0f },
    { _unit_animation_impulse_dive_right,  _actor_evade_right,   1.5f },
    { _unit_animation_impulse_evade_right, _actor_evade_right,   0.0f },
    { _unit_animation_impulse_dive_front,  _actor_evade_forward, 1.5f },
    { _unit_animation_impulse_dive_back,   _actor_evade_back,    1.5f },
    { -1,                                  -1,                   0.0f },  /* terminator */
};
