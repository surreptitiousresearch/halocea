/* actor_look_idle_timer @0x837FB26C — pick how long (ticks) an actor holds an idle look/aim/face pose.
 * Chooses the time range by timer_type (0 = facing, 1 = aim, 2 = look; >=3 degenerate), randomizes within
 * it (defaulting to 0.5s when the range is empty), scales by the held weapon's AI idle-look modifier
 * (weapon.ai_idle_look_time_modifier) and by 1.5 when looking toward something interesting, then converts
 * to ticks (*30, floored at 1).
 * DEVIATION: the timer_type>=3 branch reads an uninitialized stack value in the original; reproduced as
 * 0. Soft-float reproduced as float math. */

#include <stdint.h>
#include "headers/actor_idle_looking.h"
#include "headers/weapon_definition.h"

extern weapon_definition *actor_get_weapon_definition(int actor_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

int actor_look_idle_timer(int actor_index, actor_idle_looking *looking_definition, int16_t timer_type, uint8_t interesting_direction)
{
    weapon_definition *weapon = actor_get_weapon_definition(actor_index);

    float lower_bound;
    float upper_bound;
    if ( timer_type == 0 )
    {
        lower_bound = looking_definition->idle_facing_time_lower_bound;
        upper_bound = looking_definition->idle_facing_time_upper_bound;
    }
    else if ( timer_type == 1 )
    {
        lower_bound = looking_definition->idle_aim_time_lower_bound;
        upper_bound = looking_definition->idle_aim_time_upper_bound;
    }
    else if ( (unsigned int)timer_type >= 3 )
    {
        lower_bound = 0.0f;   /* original reads an uninitialized stack value here */
        upper_bound = 0.0f;
    }
    else
    {
        lower_bound = looking_definition->idle_look_time_lower_bound;
        upper_bound = looking_definition->idle_look_time_upper_bound;
    }

    float time;
    if ( lower_bound > 0.0f || upper_bound > 0.0f )
    {
        uint32_t *seed = get_global_random_seed_address();
        time = real_seed_random_range(seed, lower_bound, upper_bound);
    }
    else
    {
        time = 0.5f;
    }

    if ( weapon && weapon->weapon.ai_idle_look_time_modifier > 0.0f )
        time = weapon->weapon.ai_idle_look_time_modifier * time;
    if ( interesting_direction )
        time = time * 1.5f;

    int ticks = (int)(time * 30.0f);
    if ( ticks <= 1 )
        return 1;
    return ticks;
}
