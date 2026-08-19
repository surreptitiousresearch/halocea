/* actor_start_first_burst_delay @0x837B7DB0 — arm an actor's first-burst delay before it opens fire.
 * Normally a randomized delay (variant's first_burst_delay range @ def +0x80/+0x84, in seconds * 30 ticks)
 * is set into the actor's burst-delay timer (word +1524). But if the actor is set to fire immediately
 * (byte +1111), or its perceived target prop is an orphan (prop->state in
 * [_prop_state_uninspected_orphan, _prop_state_inspected_orphan]), the delay is
 * skipped (timer 0) and the immediate-fire byte (+956) is latched. Returns TRUE when a delay was set
 * (i.e. immediate fire was NOT taken).
 * Actor fields resolved to DB actor_datum members (orders.combat.override_firing_restrictions @+1111,
 * control.fire_state_timer @+1524, firing_positions.pursuit_fired_at_orphan @+956); the variant fields
 * resolved to actor_variant_definition.ranged_combat.first_burst_delay_lower/upper_bound @+0x80/+0x84. */

#include <stdint.h>
#include "headers/actor_fire_target_type.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_variant_definition.h"
#include "headers/blam_data_globals.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t actor_start_first_burst_delay(int actor_index, const actor_variant_definition *firing_variant_definition)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uint8_t immediate = actor->orders.combat.override_firing_restrictions;

    /* DEVIATION: disasm lhz control+0x1A0 / lwz control+0x1A4 = current_fire_target_type / union prop index;
     * prior source read weapon_maximum_range / current_fire_target_type (each one member early) */
    if ( actor->control.current_fire_target_type == actor_fire_target_prop )
    {
        int16_t prop_state = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index))->state;
        if ( prop_state >= _prop_state_uninspected_orphan && prop_state <= _prop_state_inspected_orphan )
        {
            immediate = 1;
            actor->firing_positions.pursuit_fired_at_orphan = 1;  /* recovered: (&firing_positions)[4] -> +956 */
        }
    }

    int set_delay = immediate;
    if ( immediate )
    {
        actor->control.fire_state_timer = 0;
    }
    else
    {
        /* recovered: *(float *)((char *)variant + 0x80/0x84) -> ranged_combat.first_burst_delay_lower/upper_bound */
        float lower_bound = firing_variant_definition->ranged_combat.first_burst_delay_lower_bound;
        float upper_bound = firing_variant_definition->ranged_combat.first_burst_delay_upper_bound;
        unsigned int *seed = get_global_random_seed_address();
        actor->control.fire_state_timer = (int)(real_seed_random_range(seed, lower_bound, upper_bound) * 30.0f);
    }
    return set_delay == 0;
}
