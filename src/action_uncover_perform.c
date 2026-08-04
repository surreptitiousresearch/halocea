/* action_uncover_perform @0x83824A60 — per-tick "uncover" action update: bails (returns 0) unless the
 * actor's +76 flag is set and both +352 and action_data+1 are clear. Builds a firing_position_evaluation_
 * context in evaluation_mode 3 ("uncover"); if the actor's stored uncover mode (action_data+8, WORD) is 1,
 * seeds a specific target point/surface/cluster from action_data+12..31 (mirroring how
 * action_guard_setup_postcombat.c and friends stash target state directly in the action union), otherwise
 * carries action_data+4's byte into use_last_visible_target_position. Re-evaluates the firing position via
 * actor_active_select_firing_position and, on success, updates a couple of small state bits (action_data+4
 * or action_data+32) based on the resulting best_firing_position's line_of_sight/path_distance_from_actor
 * versus actor_destination_tolerance(), then commits via actor_change_firing_position (setting
 * action_data+2 on success). Always returns 0.
 *
 * DEVIATION: same decompiler failure mode as action_avoid_perform.c — the ~0x664-byte
 * firing_position_evaluation_context plus the firing_position/path_state scratch buffers blow up the local
 * stack frame past what Hex-Rays can track, fabricating dozens of phantom int parameters and misreading the
 * `_RtlCheckStack12` stack-overflow probe (which leaves r3/actor_index untouched) as a call whose return
 * value becomes the "actor_index" local. Every field write below was re-derived from disasm
 * (0x83824A60-0x83824BD0): each phantom parameter's byte offset was computed as its index distance from the
 * evaluation_context's own base parameter and matched field-for-field against
 * firing_position_evaluation_context.h / firing_position.h. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position.h"
#include "headers/path_state.h"
#include "headers/uncover_state_data.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/pursuit_location_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int value, unsigned int n);

extern __int16 actor_active_select_firing_position(int actor_index, firing_position_evaluation_context *context,
        firing_position *best_firing_position, int *current_owner, path_state *area_path_state,
        unsigned __int8 *cached_path_available);
extern int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available);
extern float actor_destination_tolerance(uint16_t actor_index);

unsigned __int8 action_uncover_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uncover_state_data *action_data = &actor->state.action_data.___u0.uncover;

    if ( !actor->meta.timeslice || actor->input.vehicle_passenger || action_data->uncover_done )
        return 0;

    firing_position_evaluation_context evaluation_context;
    firing_position best_firing_position;
    path_state area_path_state;
    int current_owner;
    unsigned __int8 cached_path_available;

    memset(&evaluation_context, 0, sizeof(evaluation_context));
    evaluation_context.evaluation_mode = _firing_point_evaluation_mode_uncover;

    if ( action_data->pursuit_location.type == _pursuit_location_position )
    {
        evaluation_context.specific_target_enable = 1;
        evaluation_context.specific_target_point.n[0] = action_data->pursuit_location.position.x;
        evaluation_context.specific_target_point.n[1] = action_data->pursuit_location.position.y;
        evaluation_context.specific_target_point.n[2] = action_data->pursuit_location.position.z;
        evaluation_context.specific_target_surface_index = action_data->pursuit_location.surface_index;
        evaluation_context.specific_target_cluster_index = action_data->pursuit_location.cluster_index;
    }
    else
    {
        evaluation_context.use_last_visible_target_position = action_data->no_target_sight_available;
    }

    __int16 firing_position_index = actor_active_select_firing_position(actor_index, &evaluation_context,
            &best_firing_position, &current_owner, &area_path_state, &cached_path_available);

    if ( firing_position_index != -1 )
    {
        if ( action_data->pursuit_location.type )
        {
            if ( best_firing_position.line_of_sight == _ai_line_of_sight_clear
              && best_firing_position.path_distance_from_actor < actor_destination_tolerance(actor_index) )
                action_data->pursuit_location_inspected = 1;
        }
        else if ( best_firing_position.line_of_sight != _ai_line_of_sight_clear && best_firing_position.line_of_sight != _ai_line_of_sight_occluded )
        {
            action_data->no_target_sight_available = 1;
        }
    }

    __int16 change_result = actor_change_firing_position(actor_index, firing_position_index, &best_firing_position,
            current_owner, &area_path_state, cached_path_available);
    if ( change_result != -1 )
        action_data->uncover_exit_failure = 1;

    return 0;
}
