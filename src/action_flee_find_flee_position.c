/* action_flee_find_flee_position @0x83825358 — picks and commits a new firing position for an actor that
 * is fleeing, then (if it still has a "prop" — a tracked other unit — it is fleeing from) computes an
 * approach point along the path leading away from that prop.
 *
 * DEVIATION: the DB's own decompile fabricates ~60 phantom int parameters and reads the real actor_index/
 * flee_state_data inputs through a bogus `RtlCheckStack12()`-return-derived pointer. disasm_range
 * (0x83825358-0x838255b0) shows RtlCheckStack12 is a plain stack-probe helper (frame is ~131KB — two
 * 65676-byte path_state locals) whose return value is never used; the real inputs are r3 (actor_index,
 * preserved across the probe call) and r4 (flee_state_data*, `mr r31, r4`). This matches the 2-arg extern
 * already established at the action_flee_setup.c call site. Evaluation-context field writes were cross-
 * checked against firing_position_evaluation_context's DB-confirmed offsets (arg_F0 == &context); the two
 * "destination_point"/"end_point" pointers passed to path_state_destination/path_state_approach_point are
 * `best_firing_position.definition` reloaded as a plain value (its `position` field is offset 0, so it
 * doubles as the real_point3d* directly), confirmed via the definition->surface_index read at +0x14. */

#include <stdint.h>
#include <string.h>

#include "headers/data_array.h"
#include "headers/flee_state_data.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position.h"
#include "headers/path_state.h"
#include "headers/real_point2d.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_panic_type.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/firing_position_group_selection.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern unsigned int actor_get_firing_position_group(int actor_index, int16_t evaluation_mode, int16_t group_selection_mode);
extern int16_t actor_select_firing_position(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *best_firing_position, int *current_owner, path_state *area_path_state, uint8_t *area_path_state_valid);
extern int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available);
extern void actor_perception_find_prop_pathfinding_location(int actor_index, int prop_index);
extern void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces, int ignore_source_object_index);
extern void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index);
extern void path_input_set_target_object(path_input *input, int target_object_index);
extern void path_state_new(const path_input *input, path_state *state, path_debug_storage *debug);
extern void path_state_destination(path_state *state, const real_point3d *destination_point, int destination_surface_index, float destination_accept_radius);
extern uint8_t path_state_find(path_state *state);
/* extern corrected to match def (path_state_approach_point.c): returns int; end_point is real_point2d*. */
extern uint8_t path_state_approach_point(path_state *state, const real_point2d *end_point, int end_surface_index, uint8_t *straight_line_reference, real_point3d *approach_point_reference);

void action_flee_find_flee_position(int actor_index, flee_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);

    firing_position_evaluation_context context;
    memset(&context, 0, sizeof(context));

    context.use_last_visible_target_position = state_data->flee_from_last_visible_location;

    if (state_data->panic_type <= _actor_panic_none)
    {
        context.evaluation_data.___u0.cover.allow_occluded_points = state_data->allow_occluded_points;
        context.evaluation_mode = _firing_point_evaluation_mode_cover;
        context.maximum_search_range = definition->defensive.cover_max_distance;
        if (context.maximum_search_range <= 0.0f)
            context.maximum_search_range = 6.0f;
    }
    else
    {
        context.evaluation_mode = _firing_point_evaluation_mode_panic;
        if (state_data->forced_flee_ticks > 0)
        {
            context.allow_rejected_positions = 1;
            context.allow_outside_range = 1;
        }
        context.attractor_enable = 1;
        context.attractor_weight = 10.0f;
        context.attractor_radius = 6.0f;
    }

    context.allowed_position_mask = actor_get_firing_position_group(actor_index, context.evaluation_mode, _firing_position_group_normal);

    firing_position   best_firing_position;
    int               current_owner;
    path_state        area_path_state;
    uint8_t   area_path_state_valid;

    int16_t firing_position_index = actor_select_firing_position(actor_index, &context, &best_firing_position,
            &current_owner, &area_path_state, &area_path_state_valid);
    state_data->flee_firing_position_index = firing_position_index;

    firing_position_index = actor_change_firing_position(actor_index, firing_position_index, &best_firing_position,
            current_owner, &area_path_state, area_path_state_valid);
    state_data->flee_firing_position_index = firing_position_index;

    state_data->has_approach_point = 0;
    state_data->flee_firing_position_found_randomly =
            (firing_position_index != -1 && !area_path_state_valid) ? 1 : 0;

    if (firing_position_index != -1 && state_data->flee_prop_index != -1)
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, state_data->flee_prop_index);

        if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged)
            actor_perception_find_prop_pathfinding_location(actor_index, state_data->flee_prop_index);

        int target_object_index = (prop->vehicle_index != -1) ? prop->vehicle_index : prop->unit_index;

        path_input input;
        path_input_new(&input, definition->moving.pathfinding_radius, 0, target_object_index);
        path_input_set_start(&input, &prop->pathfinding_point, prop->pathfinding_surface_index);
        path_input_set_target_object(&input, actor->meta.unit_index);

        path_state state;
        path_state_new(&input, &state, 0);
        path_state_destination(&state, &best_firing_position.definition->position,
                best_firing_position.definition->surface_index, 0.0f);

        if (path_state_find(&state))
        {
            uint8_t straight_line;
            state_data->has_approach_point = path_state_approach_point(&state,
                    (const real_point2d *)&best_firing_position.definition->position,
                    best_firing_position.definition->surface_index,
                    &straight_line, &state_data->approach_point);
        }
    }

    state_data->find_new_flee_position = 0;
}
