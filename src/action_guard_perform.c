/* action_guard_perform @0x83823DE8 — per-tick update for an actor executing a GUARD behavior. If the actor is
 * inactive it resets the guard location type and bails; if told to stand down it also flags a reposition. When
 * the guard's current firing position has become invalid (location type 3 but no firing-position index), it is
 * cleared and a reposition is requested. On a pending reposition (actor+76 set and find_new_guard_position),
 * it discards any current firing position, selects and commits a new one via actor_select_firing_position /
 * actor_change_firing_position (evaluation mode 4 = guard), records the outcome, and arms a randomized re-check
 * timer (guard-delay range from the actor's tag, in seconds, x30 ticks/second). Always returns 0.
 *
 * DEVIATION: Hex-Rays split the RtlCheckStack12 stack probe (this frame reserves a 0x664-byte evaluation
 * context) into phantom locals v6/v8; the `*(back_chain + v8) = back_chain[0]` write is the probe artifact and
 * is dropped. v6 is the actor base register (r10), so `*(u16*)(v6+952)` is the actor's current
 * firing_position_index at actor+952. Hex-Rays also aliased the path_state out-param and its valid flag onto a
 * single 8-byte v16 buffer (path_state is larger than 8 bytes); the DB prototypes are unambiguous
 * (select: path_state*, u8*; change: path_state*, u8 by value), so separate correctly-typed locals are used.
 * guard_state_data is at actor+156; actor named fields: meta.swarm (byte 6, inactive flag), meta.timeslice
 * (byte 76, reposition trigger), input.vehicle_passenger (byte 352, stand-down). Actor tag definition
 * (TAG_INSTANCE of actor+88) +952/+956 = guard delay min/max seconds. */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/guard_state_data.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/path_state.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


extern void actor_discard_firing_position(int actor_index, int16_t firing_position_index, uint8_t temporary);
extern unsigned int actor_get_firing_position_group(int actor_index, int16_t evaluation_mode, int16_t group_selection_mode);
extern int16_t actor_select_firing_position(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *best_firing_position, int *current_owner, path_state *area_path_state, uint8_t *area_path_state_valid);
extern int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t action_guard_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    guard_state_data *guard = &actor->state.action_data.___u0.guard;
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);   /* actor+88 = variant tag index */

    if ( actor->meta.swarm )   /* inactive */
    {
        guard->guard_location_type = _actor_guard_location_current;
        return 0;
    }
    if ( actor->input.vehicle_passenger )   /* stand down */
    {
        guard->guard_location_type = _actor_guard_location_current;
        guard->find_new_guard_position = 1;
        return 0;
    }

    if ( guard->guard_location_type == _actor_guard_location_firing_position && (uint16_t)actor->firing_positions.current_position_index == 0xFFFF )   /* DEVIATION: int16_t field — the uncast compare promoted to -1 and was always false, so the stale-firing-position reset never ran; binary zero-extends, lhz r11,0x3B8(r10) @0x83823E9C + cmplwi cr6,r11,0xFFFF @0x83823EA0 */
    {
        guard->guard_location_type = _actor_guard_location_none;
        guard->find_new_guard_position = 1;
    }

    if ( actor->meta.timeslice && guard->find_new_guard_position )
    {
        if ( guard->guard_location_type == _actor_guard_location_firing_position )
        {
            uint16_t firing_position_index = actor->firing_positions.current_position_index;
            if ( firing_position_index != 0xFFFF )
                actor_discard_firing_position(actor_index, firing_position_index, 0);
        }

        firing_position_evaluation_context evaluation_context;
        memset(&evaluation_context, 0, sizeof(evaluation_context));
        evaluation_context.evaluation_mode = _firing_point_evaluation_mode_guard;
        evaluation_context.allowed_position_mask = actor_get_firing_position_group(actor_index, _firing_point_evaluation_mode_guard, 0);
        evaluation_context.allow_outside_range = 1;

        firing_position best_firing_position;
        int current_owner;
        path_state area_path_state;
        uint8_t area_path_state_valid;
        int16_t selected = actor_select_firing_position(actor_index, &evaluation_context, &best_firing_position,
                &current_owner, &area_path_state, &area_path_state_valid);
        int16_t changed = actor_change_firing_position(actor_index, selected, &best_firing_position,
                current_owner, &area_path_state, area_path_state_valid);

        guard->find_new_guard_position = 0;
        guard->has_guard_direction = 0;
        if ( changed == -1 )
        {
            guard->guard_location_type = _actor_guard_location_current;
        }
        else
        {
            guard->___u17.guard_firing_position_index = changed;
            guard->guard_location_type = _actor_guard_location_firing_position;
        }

        float delay_max = definition->firing_position.guard_position_time_upper_bound;
        float delay_min = definition->firing_position.guard_position_time_lower_bound;
        unsigned int *seed = get_global_random_seed_address();
        guard->wait_ticks = (int)(real_seed_random_range(seed, delay_min, delay_max) * 30.0f);
    }

    return 0;
}
