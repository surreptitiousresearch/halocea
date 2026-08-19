/* actor_combat_allow_special_fire_situation @0x837B88E8 — gate the "strafing" special-fire situation
 * (_actor_special_fire_situation_strafing = 3): always allowed for other situations. For strafing, look up
 * the actor's perceived enemy prop (actor->control.current_fire_target_type, used as prop index). If that
 * prop is mounted in a vehicle (vehicle_index at +0x110 != -1), allow. Otherwise, if the prop is a player
 * (player flag at +0x12E), deny; else run a collateral-damage check around the prop's body_position (+0xBC)
 * and allow only if it would catch at least 3 enemies.
 * DEVIATION: the decompiler dropped the collateral-check return path (it returned 0 unconditionally there).
 * Recovered from disassembly (0x837B899C-0x837B89C8): returns (enemy_count >= 3). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/actor_special_fire_situation.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_combat_check_collateral_damage(int actor_index, float enemy_radius, float collateral_damage_radius, const real_point3d *test_point, int16_t *threat_count_out);

uint8_t actor_combat_allow_special_fire_situation(int actor_index, int16_t special_fire_situation)
{
    if ( special_fire_situation != _actor_special_fire_situation_strafing )
        return 1;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uint16_t prop_index = (uint16_t)actor->control.___u58.current_fire_target_prop_index; /* DEVIATION: lwz r6,0x610 @0x837B8928 (32-bit) = the union's prop-index arm, not current_fire_target_type (0x60C) */
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    if ( prop->vehicle_index != -1 )
        return 1;
    if ( prop->player )
        return 0;

    int16_t enemy_count = 0;
    actor_combat_check_collateral_damage(actor_index, 6.0f, 0.0f, &prop->body_position, &enemy_count);
    return enemy_count >= 3 ? 1 : 0;
}
