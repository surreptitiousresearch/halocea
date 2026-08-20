/* actor_start_pause @0x837B9358 — set the actor's between-burst pause timer (word +1524). Picks a random
 * separation in the firing variant's burst-separation range (burst geometry +0x1C/+0x20), scales it by the
 * team difficulty value, applies the firing pattern's burst-separation modifier (firing pattern +0x4) when
 * present, adds a 1.7x factor when the actor is playfighting (external_orders.playfighting, byte +458;
 * lbz r11,0x1CA @0x837B93FC), and converts seconds to ticks (*30).
 * DEVIATION: the soft-float double juggling is reproduced as plain float math. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_burst_geometry.h"
#include "headers/actor_firing_pattern.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"

extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern void actor_combat_get_firing_parameters(int actor_index, actor_variant_definition *firing_variant_definition, actor_burst_geometry **burst_geometry_reference, actor_firing_pattern **firing_pattern_reference);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);

void actor_start_pause(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_variant_definition *firing_variant_definition = actor_combat_get_firing_variant_definition(actor_index);

    actor_burst_geometry *burst_geometry;
    actor_firing_pattern *firing_pattern;
    actor_combat_get_firing_parameters(actor_index, firing_variant_definition, &burst_geometry, &firing_pattern);

    float upper_bound = burst_geometry->burst_separation_upper_bound;   /* recovered: *(float*)(burst_geometry+0x20) -> burst_separation_upper_bound */
    float lower_bound = burst_geometry->burst_separation_lower_bound;   /* recovered: *(float*)(burst_geometry+0x1C) -> burst_separation_lower_bound */
    unsigned int *seed = get_global_random_seed_address();
    float separation = real_seed_random_range(seed, lower_bound, upper_bound);
    float team_value = game_difficulty_get_team_value(_game_difficulty_burst_separation_delay_scale, actor->meta.team_index);

    float pause = team_value * separation;
    /* recovered: *(float*)(firing_pattern+4) -> burst_separation_modifier */
    if ( firing_pattern && firing_pattern->burst_separation_modifier != 0.0f )
        pause = firing_pattern->burst_separation_modifier * (team_value * separation);
    if ( actor->external_orders.playfighting )
        pause = pause * 1.7f;

    actor->control.fire_state_timer = (int)(pause * 30.0f);
}
