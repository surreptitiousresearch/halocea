/* infection_wander_pause_time @0x838275CC — pick a randomized pause duration (in ticks, clamped to a byte,
 * 0xFF = "no limit" on overflow) for an infection-form wander movement type: 4-5s type 1, 2-2.8s type 2,
 * 0.4-1s type 3, else a flat 1s (no randomization). */

#include <stdint.h>
#include "headers/swarm_movement_mode.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t infection_wander_pause_time(int16_t movement_type)
{
    float seconds = 1.0f;

    /* movement_type is a swarm_movement_mode wander-family value (DB enum
     * $5F9722FF666D268AD4D3846D0318AB1C); ranges match infection_swarm_control's pause seeding. */
    switch ( movement_type )
    {
        case _swarm_movement_wander_noncombat:
            seconds = real_seed_random_range(get_global_random_seed_address(), 4.0f, 5.0f);
            break;
        case _swarm_movement_wander_idle:
            seconds = real_seed_random_range(get_global_random_seed_address(), 2.0f, 2.8f);
            break;
        case _swarm_movement_wander_combat:
            seconds = real_seed_random_range(get_global_random_seed_address(), 0.4f, 1.0f);
            break;
        default:
            break;
    }

    int ticks = (int)(seconds * 30.0f);
    if ( ticks > 255 )
        ticks = 0xFF;

    return (uint8_t)ticks;
}
