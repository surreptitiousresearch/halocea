/* actor_create_for_unit @0x83721350 — create (or, for swarms, recruit an existing) AI actor and bind it to a
 * unit. For swarm actors it first scans the encounter for a compatible non-full swarm actor of the same variant
 * (and squad, unless cross-squad addition is allowed) and reuses it; otherwise it spawns a fresh actor via
 * actor_new, attaches it to the encounter (or marks it encounterless), seeds its state/command-list/sequence
 * fields, and finally attaches the unit. Returns the actor index, or -1 on failure.
 *
 * Deviation: Hex-Rays invents phantom trailing argument slots and reads the four small trailing parameters
 * through those copies rather than the named ones. The real 12-arg prototype is the database's; those stores are
 * state.initial_state, state.default_state, state.command_list_index and state.noncombat_sequence_id (verified
 * against the slot offsets and the actor_action_get_default_state fallback). */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/object_damage_flags.h"
#include "headers/actor_mode.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int actor_new(int variant_definition_index);
extern void encounterless_attach_actor(int actor_index);
extern void encounter_attach_actor(int actor_index, int encounter_index, int16_t squad_index, uint8_t has_previous_team);
extern void actor_set_dormant(uint16_t actor_index, uint8_t dormant);
extern uint16_t actor_action_get_default_state(int16_t initial_state);
extern uint8_t actor_type_get_swarm(int16_t actor_type);
extern void actor_delete(int actor_index, uint8_t died);
extern void actor_attach_unit(int actor_index, int unit_index);
extern uint8_t actor_swarm_attach_unit(int actor_index, int unit_index);

int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index,
        int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads,
        int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state,
        int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id)
{
    int index = -1;
    if ( unit_index == -1 || actor_variant_definition_index == -1 )
        return index;

    char can_create = 1;
    if ( swarm )
    {
        encounter_actor_iterator iterator;
        encounter_actor_iterator_new(&iterator, encounter_index);
        actor_datum *actor = encounter_actor_iterator_next(&iterator);
        if ( actor )
        {
            while ( !actor->meta.swarm
                 || iterator.index == disallow_actor_index
                 || actor->meta.swarm_unit_count >= 16
                 || actor->meta.variant_definition_index != actor_variant_definition_index
                 || (!allow_addition_to_other_squads && actor->meta.squad_index != squad_index) )
            {
                actor = encounter_actor_iterator_next(&iterator);
                if ( !actor )
                    goto resolved;
            }
            index = iterator.index;
        }
    }
    else
    {
        unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
        if ( !unit || (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )  /* unit cannot host an actor */
            can_create = 0;
    }

resolved:
    if ( can_create )
    {
        if ( index == -1 )
        {
            index = actor_new(actor_variant_definition_index);
            if ( index == -1 )
                return index;

            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, index);
            if ( encounter_index == -1 )
            {
                encounterless_attach_actor(index);
            }
            else
            {
                if ( (encounter_index & 0xFFFF0000) == 0 )
                {
                    unsigned int encounter_identifier =
                        (unsigned int)DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->identifier;
                    encounter_index = ((encounter_identifier << 16) | (encounter_identifier >> 16))
                        | (uint16_t)encounter_index;
                }
                encounter_attach_actor(index, encounter_index, squad_index, 0);
            }

            if ( initially_braindead )
            {
                uint8_t dormant_on_create = actor->meta.active;
                actor->state.mode = _actor_mode_braindead;
                if ( dormant_on_create )
                    actor_set_dormant(index, 0);
            }
            else
            {
                actor->state.mode = _actor_mode_alert;
            }

            actor->state.default_state = default_state;
            actor->state.initial_state = initial_state;
            if ( default_state == -1 || !default_state )
                actor->state.default_state = actor_action_get_default_state(initial_state);
            actor->state.command_list_immediate = 0;
            actor->state.command_list_delay_timer = 2;
            int16_t actor_type = actor->meta.type;
            actor->state.command_list_index = initial_command_list_index;
            actor->state.noncombat_sequence_id = noncombat_sequence_id;
            if ( actor->meta.swarm != actor_type_get_swarm(actor_type) )
            {
                actor_delete(index, 0);
                return -1;
            }
        }

        if ( index == -1 )
            return index;
    }

    if ( index == -1 )
        return index;

    if ( !swarm )
    {
        actor_attach_unit(index, unit_index);
        return index;
    }
    if ( actor_swarm_attach_unit(index, unit_index) )
        return index;
    if ( !DATA_ARRAY_ELEMENT(actor_data, actor_datum, index)->meta.swarm_unit_count )
        actor_delete(index, 0);
    return -1;
}
