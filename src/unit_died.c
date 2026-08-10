/* unit_died @ 0x836D3398 — handle a unit's death. When `feigned`, it only randomly toggles the unit's
 * "feign death" flag (unit+516 bit 0x2000) against the definition's feign-death chance (def+584). For a real
 * death it marks the unit as garbage, notifies and detaches any controlling player (unit+536), actor
 * (unit+500) and swarm (unit+504) — copying the actor's type/variant into unit+820/+822 — and stamps the
 * death time (unit+1052). In both cases it clears the firing/control bits (unit+516 low nibble & throwing
 * bit), stops the active weapon, detaches from any parent/seat, drops the inventory weapons, the held
 * powerup (unit+792), grenades, and (unless suppressed by unit+652) the current weapon, then resets a few
 * per-unit speech/zoom fields. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_flags.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern void object_set_garbage(int object_index, uint8_t garbage);
extern void player_died(int player_index);
extern void actor_died(int actor_index);
extern void actor_swarm_unit_died(uint16_t actor_index, int unit_index);
extern int game_time_get(void);
extern void weapon_owner_update(int weapon_index, uint16_t control_flags, float primary_trigger);
extern void unit_detach_from_parent(int object_index);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void unit_drop_inventory_weapons(int unit_index);
extern void unit_drop_item(int unit_index, int item_index);
extern void unit_drop_grenades(int unit_index);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);

void unit_died(int unit_index, uint8_t feigned)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( feigned )
    {
        unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
        int feign_death = real_seed_random(get_global_random_seed_address()) >= (double)definition->unit.feign_death_repeat_chance;
        int flags = unit->unit.flags;
        if ( feign_death )
            unit->unit.flags = flags & ~(1u << _unit_feign_death_allowed_bit);
        else
            unit->unit.flags = flags | (1u << _unit_feign_death_allowed_bit);
    }
    else
    {
        unit->unit.feign_death_timer = 0;
        object_set_garbage(unit_index, 1u);

        int player_index = unit->unit.player_index;
        if ( player_index != -1 )
        {
            player_died(player_index);
            unit->unit.player_index = -1;
        }

        int actor_index = unit->unit.actor_index;
        if ( actor_index != -1 )
        {
            actor_datum *actor = DATUM_GET(actor_data, actor_datum, unit->unit.actor_index);
            unit->unit.fake_encounter_index = actor->meta.encounter_index;
            unit->unit.fake_squad_index = actor->meta.squad_index;  /* DEVIATION: lhz r7, 0x3A(r11) @0x836D3494 = meta.squad_index (meta 0x36), not disconnected_squad_index (actor+0x38) */
            actor_died(actor_index);
            unit->unit.actor_index = -1;
        }

        int swarm_index = unit->unit.swarm_actor_index;
        if ( swarm_index != -1 )
        {
            actor_datum *swarm_actor = DATUM_GET(actor_data, actor_datum, unit->unit.swarm_actor_index);
            unit->unit.fake_encounter_index = swarm_actor->meta.encounter_index;
            unit->unit.fake_squad_index = swarm_actor->meta.squad_index;  /* DEVIATION: lhz r7, 0x3A(r11) @0x836D34D0 = meta.squad_index (meta 0x36), not disconnected_squad_index (actor+0x38) */
            actor_swarm_unit_died(swarm_index, unit_index);
            unit->unit.swarm_actor_index = -1;
        }

        unit->unit.time_of_death = game_time_get();
    }

    int flags = unit->unit.flags;
    int active_slot = (uint16_t)unit->unit.current_weapon_index;
    unit->unit.control_flags = 0;
    unit->unit.flags = flags & ~((1u << _unit_actively_controlled_bit) | (1u << _unit_active_camouflaged_bit));
    if ( active_slot != 0xFFFF )
    {
        int active_weapon = -1;
        unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int slot = unit_data->unit.current_weapon_index;
        if ( slot != -1 )
            active_weapon = unit_data->unit.weapon_object_indices[slot];
        weapon_owner_update(active_weapon, 0, 0.0f);
    }

    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_data->unit.flags &= ~(1u << _unit_running_blindly_bit);

    if ( unit->object.parent_object_index != -1 )
    {
        if ( (uint16_t)unit->unit.parent_seat_index == 0xFFFF )
            unit_detach_from_parent(unit_index);
        else
            unit_exit_seat_end(unit_index, 0, 0, 0);
    }

    unit->unit.speech.queued.priority = 0;
    unit_drop_inventory_weapons(unit_index);

    unit_datum *unit_data2 = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int powerup = unit_data2->unit.equipment_object_index;
    if ( powerup != -1 )
    {
        unit_drop_item(unit_index, powerup);
        unit_data2->unit.equipment_object_index = -1;
    }

    unit_drop_grenades(unit_index);
    if ( !unit->unit.weapon_drop_delay_ticks )
        unit_drop_current_weapon(unit_index, 1u);

    uint8_t throw_state = unit->unit.grenade_throw_state;
    unit->unit.animation.overlay_action_animation.index = -1;
    unit->unit.animation.action_animation.index = -1;
    unit->unit.melee_attack_state = _unit_melee_attack_none;
    if ( throw_state == 1 )
        unit->unit.grenade_throw_state = _grenade_throw_state_none;
}
