/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* unit_died_client @0x836D3610 — client-side reaction to a unit dying (or feigning death). For a feign, it
 * rolls the unit definition's feign-persistence chance (+584) against the global random stream and sets or clears
 * the "feigning death" unit flag (0x2000) accordingly. For a real death it tears down the unit's live links:
 * notifies the controlling player (player_died), the controlling AI actor (actor_died) and the swarm actor
 * (actor_swarm_unit_died) — stashing each actor's encounter/squad index into the unit first — clears those index
 * slots and stamps the death time. The common tail (run for both cases) clears the primary-weapon / dual-wield
 * flag bits, notifies the current weapon's owner-update, clears the "just died" flag bit, exits any seat or
 * detaches from a parent, drops inventory / held item / current weapon, and resets a couple of animation/seat
 * slots.
 *
 * The unit object datum is read at raw offsets (corpus convention for live datums); the controlling actor is
 * typed against actor_datum only for the encounter/squad copy. Offsets confirmed against the disassembly. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_flags.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/blam_data_globals.h"


#include <stdint.h>
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern void player_died(int player_index);
extern void actor_died(int actor_index);
extern void actor_swarm_unit_died(int actor_index, int unit_index);
extern int game_time_get(void);
extern void weapon_owner_update(int weapon_index, uint16_t control_flags, float primary_trigger);
extern void unit_detach_from_parent(int object_index);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void unit_drop_inventory_weapons(int unit_index);
extern void unit_drop_item(int unit_index, int item_index);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);


void unit_died_client(int unit_index, uint8_t feigned)
{
    unit_datum *unit = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( feigned )
    {
        unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
        unsigned int *seed = get_global_random_seed_address();
        if ( real_seed_random(seed) >= definition->unit.feign_death_repeat_chance )
            unit->unit.flags &= ~(1u << _unit_feign_death_allowed_bit);
        else
            unit->unit.flags |= (1u << _unit_feign_death_allowed_bit);
    }
    else
    {
        unit->unit.feign_death_timer = 0;

        int player_index = unit->unit.player_index;
        if ( player_index != -1 )
        {
            player_died(player_index);
            unit->unit.player_index = -1;
        }

        int actor_index = unit->unit.actor_index;
        if ( actor_index != -1 )
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
            unit->unit.fake_encounter_index = (int16_t)actor->meta.encounter_index;
            unit->unit.fake_squad_index = actor->meta.squad_index;
            actor_died(actor_index);
            unit->unit.actor_index = -1;
        }

        int swarm_actor_index = unit->unit.swarm_actor_index;
        if ( swarm_actor_index != -1 )
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, swarm_actor_index);
            unit->unit.fake_encounter_index = (int16_t)actor->meta.encounter_index;
            unit->unit.fake_squad_index = actor->meta.squad_index;
            actor_swarm_unit_died(swarm_actor_index, unit_index);
            unit->unit.swarm_actor_index = -1;
        }

        unit->unit.time_of_death = game_time_get();
    }

    int16_t current_weapon_slot = unit->unit.current_weapon_index;
    unit->unit.control_flags = 0;
    /* corrected: 0x11 = bits 0 and 4; DB $C43855A4... = actively_controlled + active_camouflaged (not "primary-weapon/dual-wield") */
    unit->unit.flags &= ~((1u << _unit_actively_controlled_bit) | (1u << _unit_active_camouflaged_bit));
    if ( (uint16_t)current_weapon_slot != 0xFFFF )
    {
        int weapon_object_index = -1;
        if ( current_weapon_slot != -1 )
            weapon_object_index = unit->unit.weapon_object_indices[current_weapon_slot];
        weapon_owner_update(weapon_object_index, 0, 0.0f);
    }

    unit->unit.flags &= ~(1u << _unit_running_blindly_bit);

    if ( unit->object.parent_object_index != -1 )   /* object.parent_object_index */
    {
        if ( (uint16_t)unit->unit.parent_seat_index == 0xFFFF )
            unit_detach_from_parent(unit_index);
        else
            unit_exit_seat_end(unit_index, 0, 1, 0);
    }

    unit->unit.speech.queued.priority = 0;
    unit_drop_inventory_weapons(unit_index);

    int held_item_index = unit->unit.equipment_object_index;
    if ( held_item_index != -1 )
    {
        unit_drop_item(unit_index, held_item_index);
        unit->unit.equipment_object_index = -1;
    }

    if ( !unit->unit.weapon_drop_delay_ticks )
        unit_drop_current_weapon(unit_index, 1);

    uint8_t pending_seat_reset = unit->unit.grenade_throw_state;
    unit->unit.animation.overlay_action_animation.index = -1;
    unit->unit.animation.action_animation.index = -1;
    unit->unit.melee_attack_state = _unit_melee_attack_none;
    if ( pending_seat_reset == 1 )
        unit->unit.grenade_throw_state = _grenade_throw_state_none;
}
