/* infection_wander_move_time @0x83827670 — pick a randomized wander-move duration (in ticks, clamped to a
 * byte) for a flood infection form: movement_type 1 rolls uniformly in [1.0, 2.5), movement_type 2 or 3 rolls
 * in [0.6, 1.8), any other movement_type gets a flat 1.0 — each multiplied by 30 ticks/second and clamped to
 * 255. */

#include <stdint.h>
#include "headers/swarm_movement_mode.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t infection_wander_move_time(int16_t movement_type)
{
    float scale = 1.0f;

    if ( movement_type == _swarm_movement_wander_noncombat )
    {
        scale = real_seed_random_range(get_global_random_seed_address(), 1.0f, 2.5f);
    }
    else if ( movement_type > _swarm_movement_wander_noncombat && movement_type <= _swarm_movement_wander_combat )
    {
        scale = real_seed_random_range(get_global_random_seed_address(), 0.6f, 1.8f);
    }

    int ticks = (int)(scale * 30.0f);
    /* DEVIATION: disasm clamps to 0xFF (li r11,0xFF @0x838276F4), not -1. */
    if ( ticks > 255 )
        ticks = 0xFF;

    return (uint8_t)ticks;
}
