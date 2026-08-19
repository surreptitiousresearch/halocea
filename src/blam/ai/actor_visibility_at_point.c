/* actor_visibility_at_point @0x837D8AA0 — decide how well the actor can see a target at `position`. The
 * character's vision range is scaled by combat-knowledge type, ambient lighting, and fog, then the target must
 * fall inside that range. Unless the actor is a swarm or frustum testing is disabled, the target's bearing is
 * checked against the actor's view frustum (via actor_get_vision_distances) to obtain the full/partial sight
 * distances. Returns 0 (unseen), 1 (seen), or 2/3 graded by range when no prior line-of-sight is required.
 *
 * The frustum bearing math is transcribed from Hex-Rays' packed-register form: the target delta is projected
 * onto the actor's orientation basis (floats at actor +0x18C..) to give forward/left/up components; pitch =
 * atan2(up, horizontal_magnitude) gates the vertical frustum, and yaw = atan2(left, forward) feeds the
 * horizontal vision lookup. The firing variant's modified_vision_range is reached through the typed
 * actor_variant_definition->ranged_combat member.
 */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_knowledge.h"
#include <math.h>
#include "headers/ai_line_of_sight.h"
#include "headers/actor_perception_type.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"

extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern float scenario_fog_at_point(const location *viewer_location, const real_point3d *viewer_point, const real_point3d *point);
extern void actor_get_vision_distances(int actor_index, float maximum_vision_distance, float perception_factor, float horizontal_angle, float *full_distance_reference, float *partial_distance_reference);

int16_t actor_visibility_at_point(int actor_index, actor_position_data *sense_position,
                              const real_point3d *position, char lighting, int16_t line_of_sight,
                              uint8_t use_frustum, uint8_t store_debugging_information,
                              int16_t combat_knowledge_type)
{
    if ( line_of_sight && line_of_sight != _ai_line_of_sight_occluded )
        return 0;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *firing_variant = actor_combat_get_firing_variant_definition(actor_index);

    float vision_range = character->perception.maximum_vision_distance;
    /* recovered: raw +0x18 was wrong (that offset is unit_reference); binary reads ranged_combat.modified_vision_range (variant+0x150) */
    if ( firing_variant->ranged_combat.modified_vision_range > 0.0f )
        vision_range = firing_variant->ranged_combat.modified_vision_range;

    float lighting_scale = 1.0f;
    float knowledge_scale;
    if ( combat_knowledge_type == _actor_knowledge_guard )
        knowledge_scale = 0.60000002f;
    else if ( combat_knowledge_type == _actor_knowledge_searching )
        knowledge_scale = 0.80000001f;
    else
        knowledge_scale = combat_knowledge_type ? 1.0f /* _actor_knowledge_definite */ : 0.40000001f /* _actor_knowledge_noncombat */;

    float effective_range = knowledge_scale * vision_range;
    float dy = position->n[1] - sense_position->head_position.n[1];
    float dz = position->n[2] - sense_position->head_position.n[2];
    float dx = position->n[0] - sense_position->head_position.n[0];
    float distance_squared = (dx * dx) + ((dz * dz) + (dy * dy));
    if ( distance_squared >= (effective_range * effective_range) )
        return 0;

    if ( (*(unsigned int *)character & (1u << _actor_definition_can_see_in_darkness_bit)) == 0 )  /* actor_definition.flags: not can_see_in_darkness; _DWORD->unsigned int */
    {
        if ( lighting )
        {
            if ( lighting == 1 )
                lighting_scale = 0.69999999f;
        }
        else
        {
            lighting_scale = 0.30000001f;
        }
    }

    float fog = scenario_fog_at_point(&sense_position->body_location, &sense_position->head_position, position);
    if ( fog > 0.80000001f )
    {
        lighting_scale = 0.15000001f;
    }
    else
    {
        if ( fog > 0.2f )
            lighting_scale = ((0.80000001f - fog) * lighting_scale) * 1.6666666f;
        if ( lighting_scale <= 0.15000001f )
            lighting_scale = 0.15000001f;
    }
    if ( distance_squared >= ((lighting_scale * effective_range) * (lighting_scale * effective_range)) )
        return 0;

    float partial_distance, full_distance;
    if ( actor->meta.swarm || !use_frustum )  /* swarm or frustum disabled: use plain range bands */
    {
        partial_distance = lighting_scale * effective_range;
        full_distance = (lighting_scale * effective_range) * 0.69999999f;
    }
    else
    {
        /* project the target delta onto the actor's orientation basis (actor +0x18C row-major 3x3) */
        float forward = (actor->input.looking_vector.n[0] * dx)
                      + ((actor->input.looking_vector.n[2] * dz) + (actor->input.looking_vector.n[1] * dy));
        float left = (actor->input.looking_left_vector.n[0] * dx)
                   + ((actor->input.looking_left_vector.n[2] * dz) + (actor->input.looking_left_vector.n[1] * dy));
        float up = (actor->input.looking_up_vector.n[0] * dx)
                 + ((actor->input.looking_up_vector.n[2] * dz) + (actor->input.looking_up_vector.n[1] * dy));
        float horizontal_magnitude = __fsqrts((left * left) + (forward * forward));
        float pitch = atan2(up, horizontal_magnitude);
        if ( pitch > 0.52359879f || pitch < -0.78539819f )  /* +30deg / -45deg vertical frustum */
        {
            full_distance = 0.0f;
            partial_distance = 0.0f;
        }
        else
        {
            float yaw = atan2(left, forward);
            actor_get_vision_distances(actor_index, effective_range, lighting_scale, __fabs(yaw),
                                       &full_distance, &partial_distance);
        }
    }

    if ( line_of_sight || distance_squared >= (full_distance * full_distance) )
    {
        if ( distance_squared < (partial_distance * partial_distance) )
            return actor_perception_partial;
        return actor_perception_none;
    }
    if ( distance_squared >= 36.0f )
        return actor_perception_full;
    return actor_perception_unmistakable;
}
