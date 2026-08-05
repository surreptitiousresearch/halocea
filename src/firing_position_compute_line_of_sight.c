/* firing_position_compute_line_of_sight @0x837EEF60 — evaluate whether `actor_index` has line of sight from
 * a candidate firing position to the evaluation context's current target, storing the result (an
 * ai_test_line_of_sight verdict) into firing_position->line_of_sight.
 *
 * Two paths:
 *   - evaluation_mode == _firing_point_evaluation_mode_pursue (a "quick check from the actor's own current position" query): bails with
 *     line_of_sight = 4 if the position is farther than 6 units of path distance from the actor; otherwise
 *     tests line of sight from the actor's own head position/cluster straight to the target.
 *   - all other modes: estimates the unit's body position at the firing position (mode 2 for
 *     modes 1/2, mode 3 with a facing/gun-offset derived from the target when a gun-offset-stand is
 *     configured — falling back to the actor's own facing (+372) when that facing degenerates to a
 *     zero-length horizontal vector, mode 1 otherwise), then tests line of sight from the estimated
 *     position to the target line-of-sight position.
 *
 * DEVIATION: the decompiler rendered the final `ignore_vehicles` argument as
 * `v6[344] + 1 - (v6[344] + (*(_DWORD*)(v6+86) == -1))` — an algebraically self-cancelling expression
 * (reduces to `1 - (*(int*)(actor+344) == -1)`) built from a single PowerPC "nonzero-to-bool" idiom
 * (addic/subfe) that Hex-Rays split into two fake reads of the same field. Disasm
 * (0x837EF0EC-0x837EF0FC) confirms only one 4-byte read at actor+344 occurs; simplified to
 * `*(int*)(actor + 344) != -1`, matching the identical, already-established pattern in
 * actor_combat_reaim_grenade.c. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/ai_line_of_sight.h"
#include "headers/ai_line_of_sight_mode.h"
#include "headers/unit_estimate_position_type.h"
#include "headers/blam_data_globals.h"


extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern float normalize2d(real_vector2d *v);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);

void firing_position_compute_line_of_sight(uint16_t actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    int16_t evaluation_mode = evaluation_context->evaluation_mode;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int unit_index = actor->meta.unit_index;

    const real_point3d *line_of_sight_source;
    int16_t source_cluster_index;
    const real_point3d *line_of_sight_target;
    int16_t target_cluster_index;
    int16_t mode;
    uint8_t test_line_of_fire;
    int ignore_object_index;
    real_point3d estimated_position;

    if ( evaluation_mode == _firing_point_evaluation_mode_pursue )
    {
        if ( firing_position->path_distance_from_actor >= 6.0f )
        {
            firing_position->line_of_sight = _ai_line_of_sight_obstructed;
            return;
        }

        int16_t estimate_mode = 1;
        unit_estimate_position(unit_index, estimate_mode, &firing_position->definition->position, nullptr, nullptr,
                &estimated_position);

        line_of_sight_source = &actor->input.position.head_position;
        source_cluster_index = actor->input.position.body_location.cluster_index; /* recovered: *((__int16*)actor+164) -> input.position.body_location.cluster_index */
        line_of_sight_target = &estimated_position;
        target_cluster_index = firing_position->definition->cluster_index;
        ignore_object_index = -1;
        test_line_of_fire = 0;
        mode = _ai_line_of_sight_normal;
    }
    else
    {
        int16_t estimate_mode;
        real_vector3d *desired_facing;
        real_vector3d *desired_gun_offset;
        real_vector3d gun_offset_direction; /* only n[0]/n[1] are normalized; n[2] handled separately below */

        if ( evaluation_mode == _firing_point_evaluation_mode_panic || evaluation_mode == _firing_point_evaluation_mode_cover )
        {
            estimate_mode = _unit_estimate_head_crouching;
            desired_facing = nullptr;
            desired_gun_offset = nullptr;
        }
        else if ( evaluation_context->has_gun_offset_stand )
        {
            estimate_mode = _unit_estimate_gun_position;
            desired_gun_offset = &evaluation_context->gun_offset_stand;

            gun_offset_direction.n[0] = evaluation_context->target_head_position.n[0]
                    - firing_position->definition->position.n[0];
            gun_offset_direction.n[1] = evaluation_context->target_head_position.n[1]
                    - firing_position->definition->position.n[1];
            gun_offset_direction.n[2] = evaluation_context->target_head_position.n[2]
                    - firing_position->definition->position.n[2];

            if ( normalize2d((real_vector2d *)&gun_offset_direction) <= 0.0f )
            {
                desired_facing = &actor->input.facing_vector; /* degenerate horizontal direction: use the actor's own facing */
            }
            else
            {
                gun_offset_direction.n[2] = 0.0f;
                desired_facing = &gun_offset_direction;
            }
        }
        else
        {
            estimate_mode = 1;
            desired_facing = nullptr;
            desired_gun_offset = nullptr;
        }

        unit_estimate_position(unit_index, estimate_mode, &firing_position->definition->position, desired_facing,
                desired_gun_offset, &estimated_position);

        mode = (evaluation_context->evaluation_mode != _firing_point_evaluation_mode_fight && (uint16_t)evaluation_context->evaluation_mode <= _firing_point_evaluation_mode_uncover)
                ? _ai_line_of_sight_expand_source : _ai_line_of_sight_normal;
        test_line_of_fire = 1;
        ignore_object_index = evaluation_context->target_vehicle_index;
        target_cluster_index = evaluation_context->target_cluster_index;
        line_of_sight_target = &evaluation_context->target_line_of_sight_position;
        line_of_sight_source = &estimated_position;
        source_cluster_index = firing_position->definition->cluster_index;
    }

    firing_position->line_of_sight = ai_test_line_of_sight(line_of_sight_source, source_cluster_index,
            line_of_sight_target, target_cluster_index, mode, test_line_of_fire, ignore_object_index,
            actor->input.vehicle_index != -1);
}
