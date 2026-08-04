/* actor_active_select_firing_position @0x837F1600 — top-level firing-position pick for an active actor. Only
 * runs when the actor is in an encounter. It queries three firing-position group masks (the evaluation-mode
 * default plus group-selection modes 2 and 1): if the combined "modes 2|1" mask is no broader than the default
 * it just constrains the search to the default, otherwise it makes the default the preferred set (weight 8) and
 * allows the wider mask. It also lets the search fall outside range / accept rejected positions, then calls
 * actor_select_firing_position. On success it flips the actor's "position group left" flag if the chosen
 * position is outside the default group, and returns. On failure it rebuilds a best_firing_position from the
 * actor's remembered firing-position index (if any), scoring it against the target; if forced evaluation still
 * rejects it, the remembered index is cleared and -1 returned.
 *
 * DEVIATION: the three actor_get_firing_position_group calls all pass evaluation_context->evaluation_mode as the
 * middle arg (disasm 0x837F1650-0x837F1680: r4 loaded once from context+4, reused; only group_selection_mode
 * 0/2/1 differs) — Hex-Rays left the reused arg as phantom uninitialized locals. The actor+152 flag update is
 * the `cntlzw(x) & 0x20 != 0  <=>  x == 0` idiom (sets the byte to whether it was zero). FLT_MAX is the literal
 * 3.4028235e38f. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_definition.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/path_state.h"
#include "headers/scenario.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"


extern unsigned int actor_get_firing_position_group(uint16_t actor_index, int16_t evaluation_mode, int16_t group_selection_mode);
extern __int16 actor_select_firing_position(int actor_index,
        firing_position_evaluation_context *evaluation_context, firing_position *best_firing_position,
        int *current_owner, path_state *area_path_state, unsigned __int8 *area_path_state_valid);
extern uint8_t firing_position_forced_evaluation(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);

__int16 actor_active_select_firing_position(int actor_index,
        firing_position_evaluation_context *evaluation_context, firing_position *best_firing_position,
        int *current_owner, path_state *area_path_state, unsigned __int8 *area_path_state_valid)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.encounter_index == -1 )
        return -1;

    unsigned int default_group = actor_get_firing_position_group(actor_index,
            evaluation_context->evaluation_mode, 0);
    unsigned int group_mode2 = actor_get_firing_position_group(actor_index,
            evaluation_context->evaluation_mode, 2);
    unsigned int group_mode1 = actor_get_firing_position_group(actor_index,
            evaluation_context->evaluation_mode, 1);
    if ( (group_mode2 | group_mode1) <= default_group )
    {
        evaluation_context->allowed_position_mask = default_group;
    }
    else
    {
        evaluation_context->preferred_groups = default_group;
        evaluation_context->allowed_position_mask = group_mode2 | group_mode1;
        evaluation_context->preferred_weight = 8.0f;
    }
    evaluation_context->allow_outside_range =
            actor->firing_positions.current_position_index == (__int16)-1 || !actor->firing_positions.current_position_found_outside_range;
    evaluation_context->allow_rejected_positions = 1;

    int result = actor_select_firing_position(actor_index, evaluation_context, best_firing_position,
            current_owner, area_path_state, area_path_state_valid);
    if ( (__int16)result != -1 )
    {
        if ( ((1 << best_firing_position->definition->group_index) & default_group) == 0 )
            actor->state.searching = (unsigned __int8)(actor->state.searching == 0);   /* cntlzw(x) & 0x20 != 0  <=>  x == 0 */
        return (__int16)result;
    }

    /* selection failed: fall back to the actor's remembered firing position, if it has one */
    unsigned __int16 remembered_index = (unsigned __int16)actor->firing_positions.current_position_index;
    if ( remembered_index != 0xFFFF && actor->firing_positions.current_position_found_outside_range )
    {
        unsigned __int16 encounter_index = (unsigned __int16)actor->meta.encounter_index;
        firing_position_definition *firing_positions =
                (firing_position_definition *)((encounter_definition *)global_scenario->ai_encounters.address)
                [encounter_index].firing_positions.address;
        firing_position_definition *definition = &firing_positions[(__int16)remembered_index];
        __int16 final_index = remembered_index;

        best_firing_position->original_index = remembered_index;
        best_firing_position->path_distance_from_actor = 3.4028235e38f;
        best_firing_position->line_of_sight = _ai_line_of_sight_clear;
        best_firing_position->path_distance_to_target = 3.4028235e38f;
        best_firing_position->definition = definition;
        best_firing_position->path_closest_approach_to_target = 3.4028235e38f;
        best_firing_position->path_direction_from_target = *global_zero_vector3d;
        best_firing_position->path_direction_from_actor = *global_zero_vector3d;

        if ( evaluation_context->has_target )
        {
            float dx = definition->position.n[0] - evaluation_context->target_point.n[0];
            float dy = definition->position.n[1] - evaluation_context->target_point.n[1];
            float dz = definition->position.n[2] - evaluation_context->target_point.n[2];
            best_firing_position->linear_distance_squared_to_target = dy * dy + (dx * dx + dz * dz);
        }
        else
        {
            best_firing_position->linear_distance_squared_to_target = 0.0f;
        }

        if ( !firing_position_forced_evaluation(actor_index, evaluation_context, best_firing_position) )
        {
            final_index = -1;
            actor->firing_positions.current_position_index = -1;
        }
        *current_owner = -1;
        *area_path_state_valid = 0;
        return final_index;
    }
    return (__int16)result;
}
