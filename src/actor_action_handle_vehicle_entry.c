/* actor_action_handle_vehicle_entry @0x837F4508 — decide whether an actor should board a vehicle this tick
 * and, if so, start the vehicle-entry action. Two candidate sources are searched, nearest-first:
 *   1. If the unit definition allows it (flag 0x1000), the actor's own props (units it is aware of): any
 *      friendly player-driven vehicle whose driver seat is that prop's unit, within 10 world units.
 *   2. The script-registered ai_globals.enterable_vehicles list, filtered by per-vehicle radius, team mask,
 *      actor-type mask, and an explicit allowed-AI-index list.
 * On a hit it calls action_vehicle_setup_impromptu and, if that succeeds, switches the actor to action
 * type 9 (enter vehicle).
 *
 * The opaque actor behaviour payload is accessed via typed struct fields on actor_datum *actor.
 *
 * DEVIATIONS (disasm-resolved, 0x837F4508-0x837F4948):
 *  - action_vehicle_setup_impromptu's float args (attempt/continue distance) are f1/f2 (8.0/10.0 for the
 *    prop path; radius+3 / radius+6 for the enterable path) — the decompiler's fp locals.
 *  - Its 5th argument (state_data) register r5 is never initialised in this function; the shipped code
 *    passes a leftover value, so it is reproduced as an uninitialised local (the callee ignores it here).
 *  - The allowed-AI-index match uses the cntlzw==0 idiom for the low-16 compare (catalog #4). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/prop_iterator.h"
#include "headers/ai_globals.h"
#include "headers/action_state_data.h"
#include "headers/vehicle_state_data.h"
#include "headers/real_point3d.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_action.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actor_action_allowed_to_enter_vehicle(int actor_index, int vehicle_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern unsigned __int8 action_vehicle_setup_impromptu(int actor_index, int vehicle_index, float attempt_distance,
        float continue_distance, vehicle_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

#define VEHICLE_ENTRY_FLT_MAX 3.4028235e38f

unsigned __int8 actor_action_handle_vehicle_entry(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
    int now = game_time_get();

    __int16 current_action = actor->state.action;
    /* recovered: *(__int16 *)((char*)&action_data + 12) -> flee.panic_type (action 4 == flee) */
    if ( current_action == actor_action_flee && actor->state.action_data.___u0.flee.panic_type > _actor_panic_none )
        return 0;
    if ( current_action == actor_action_obey )
        return 0;

    int last_attempt_time = actor->emotions.last_vehicle_check_time;
    if ( last_attempt_time != -1 && last_attempt_time + 45 >= now )
        return 0;
    actor->emotions.last_vehicle_check_time = now;

    float best_distance_squared = VEHICLE_ENTRY_FLT_MAX;
    int best_vehicle_index = -1;
    float attempt_distance = VEHICLE_ENTRY_FLT_MAX;
    float continue_distance = VEHICLE_ENTRY_FLT_MAX;
    vehicle_state_data *state_data;   /* passed uninitialised — see file header */

    if ( (actor_def->flags & (1u << _actor_definition_helps_players_in_vehicles_bit)) != 0 )
    {
        prop_iterator iterator;
        prop_iterator_new(&iterator, actor_index);
        prop_datum *prop = prop_iterator_next(&iterator);
        if ( prop )
        {
            do
            {
                if ( prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged
                  && prop->player && !prop->enemy && prop->vehicle_index != -1
                  && actor_action_allowed_to_enter_vehicle(actor_index, prop->vehicle_index) )
                {
                    vehicle_datum *vehicle = object_try_and_get_and_verify_type(prop->vehicle_index, object_mask_vehicle);
                    if ( vehicle && vehicle->unit.driver_object_index == prop->unit_index )
                    {
                        real_point3d origin;
                        object_get_origin(prop->vehicle_index, &origin);
                        float dy = origin.n[1] - actor->input.position.body_position.n[1];
                        float dz = origin.n[2] - actor->input.position.body_position.n[2];
                        float dx = origin.n[0] - actor->input.position.body_position.n[0];
                        float distance_squared = ((dy * dy)
                                + ((dz * dz) + (dx * dx)));
                        if ( distance_squared < 100.0f && distance_squared < best_distance_squared )
                        {
                            best_vehicle_index = prop->vehicle_index;
                            best_distance_squared = (prop->distance * prop->distance);
                            attempt_distance = 8.0f;
                            continue_distance = 10.0f;
                        }
                    }
                }
                prop = prop_iterator_next(&iterator);
            }
            while ( prop );

            if ( best_vehicle_index != -1 )
                goto setup_entry;
        }
    }

    if ( actor->state.certain_combat_timer < 60 )
        return 0;
    if ( ai_globals->enterable_vehicle_count <= 0 )
        return 0;

    for ( int slot = 0; slot < ai_globals->enterable_vehicle_count; slot = (__int16)(slot + 1) )
    {
        ai_vehicle_enterable *enterable = &ai_globals->enterable_vehicles[slot];
        if ( !object_try_and_get_and_verify_type(enterable->vehicle_index, object_mask_vehicle) )
            continue;
        if ( !actor_action_allowed_to_enter_vehicle(actor_index, enterable->vehicle_index) )
            continue;

        real_point3d origin;
        object_get_origin(enterable->vehicle_index, &origin);
        float dx = actor->input.position.body_position.n[0] - origin.n[0];
        float dz = actor->input.position.body_position.n[2] - origin.n[2];
        float dy = actor->input.position.body_position.n[1] - origin.n[1];
        float distance_squared = ((dx * dx) + ((dz * dz) + (dy * dy)));
        if ( distance_squared >= best_distance_squared
          || (enterable->radius != VEHICLE_ENTRY_FLT_MAX
           && distance_squared > (enterable->radius * enterable->radius)) )
            continue;

        __int16 team_bitmask = enterable->team_bitmask;
        if ( team_bitmask > 0 )
        {
            __int16 actor_team = actor->meta.team_index;
            if ( actor_team == -1 || ((1 << actor_team) & team_bitmask) == 0 )
                continue;
        }
        __int16 actor_type_bitmask = enterable->actor_type_bitmask;
        if ( actor_type_bitmask > 0 && ((1 << (unsigned __int16)actor->meta.type) & actor_type_bitmask) == 0 )
            continue;

        __int16 ai_indices_count = enterable->ai_indices_count;
        unsigned __int8 matched;
        if ( ai_indices_count <= 0 )
        {
            /* no allowed-index restriction: any actor may enter */
            goto select_vehicle;
        }
        matched = 0;
        for ( int i = 0; i < ai_indices_count; i = (__int16)(i + 1) )
        {
            unsigned int ai_index = enterable->ai_indices[i];
            if ( ai_index == -1 )
                continue;
            /* low-16 match via cntlzw==0 idiom */
            matched = (unsigned __int16)(*(unsigned __int16 *)&actor->meta.encounter_index ^ ai_index) == 0;
            if ( matched )
            {
                if ( ai_index >> 30 == 1 )
                {
                    matched = (unsigned __int8)(((ai_index >> 8) & 0xFF) == actor->meta.platoon_index);
                }
                else if ( ai_index >> 30 == 2 )
                {
                    matched = (unsigned __int8)(((ai_index >> 8) & 0xFF) == actor->meta.squad_index);
                }
                /* types 0 and 3: the low-16 match alone is sufficient */
            }
            if ( matched )
                break;
        }
        if ( !matched )
            continue;

select_vehicle:
        best_vehicle_index = enterable->vehicle_index;
        {
            float sx = actor->input.position.body_position.n[0] - origin.n[0];
            float sz = actor->input.position.body_position.n[2] - origin.n[2];
            float sy = actor->input.position.body_position.n[1] - origin.n[1];
            best_distance_squared = ((sx * sx) + ((sz * sz) + (sy * sy)));
        }
        attempt_distance = (enterable->radius + 3.0f);
        continue_distance = (enterable->radius + 6.0f);
    }

    if ( best_vehicle_index == -1 )
        return 0;

setup_entry:
    if ( !action_vehicle_setup_impromptu(actor_index, best_vehicle_index, attempt_distance, continue_distance,
            state_data) )
        return 0;

    action_state_data new_action_data;
    actor_action_change(actor_index, actor_action_vehicle, &new_action_data);
    return 1;
}