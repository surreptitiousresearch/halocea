/* aim_assist_compute_target @0x836C5C60 — fill in the aim-assist geometry for a candidate target object.
 * Computes the object's autoaim point (object_compute_autoaim_target), the vector and unit direction from
 * the shooter's position to it, the distance, and the angle between the aim direction and the target
 * direction. With parameters supplied, the autoaim and magnetism levels are the composite angle/distance
 * attenuations; magnetism is additionally scaled by the player-control aim-assist coefficient when the
 * object's definition opts in (flag 0x80000). Returns 1 if either assist level is positive, else 0.
 *
 * The object definition flags are read through the standard TAG_INSTANCE idiom on the object's tag index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_globals_player_control.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/aim_assist_target.h"
#include "headers/aim_assist_parameters.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/game_globals_tag.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"


extern void object_compute_autoaim_target(int object_index, const real_point3d *position, const real_vector3d *direction, real_point3d *target);
extern float compute_composite_attenuation(float angle, float maximum_angle, float distance, float maximum_distance);
extern double acos(double x);

uint8_t aim_assist_compute_target(const aim_assist_parameters *parameters, int object_index, const real_point3d *position, const real_vector3d *direction, aim_assist_target *target)
{
    target->object_index = object_index;
    object_compute_autoaim_target(object_index, position, direction, &target->position);

    target->vector.n[0] = target->position.n[0] - position->n[0];
    target->vector.n[1] = target->position.n[1] - position->n[1];
    target->vector.n[2] = target->position.n[2] - position->n[2];
    target->direction = target->vector;

    float magnitude = __fsqrts(target->direction.n[2] * target->direction.n[2]
                               + (target->direction.n[0] * target->direction.n[0]
                                  + target->direction.n[1] * target->direction.n[1]));
    if (__fabs(magnitude) < 0.000099999997f)
    {
        magnitude = 0.0f;
    }
    else
    {
        float inverse = 1.0f / magnitude;
        target->direction.n[0] *= inverse;
        target->direction.n[1] *= inverse;
        target->direction.n[2] *= inverse;
    }
    target->distance = magnitude;

    float cosine = direction->n[0] * target->direction.n[0]
                   + (direction->n[1] * target->direction.n[1] + direction->n[2] * target->direction.n[2]);
    if (cosine < -1.0f)
        cosine = -1.0f;
    else if (cosine > 1.0f)
        cosine = 1.0f;
    target->angle = (float)acos(cosine);

    if (parameters)
    {
        float distance = target->distance;
        target->autoaim_level = compute_composite_attenuation(target->angle, parameters->autoaim_angle,
                                                              distance, parameters->autoaim_distance);
        float magnetism = compute_composite_attenuation(target->angle, parameters->magnetism_angle,
                                                        distance, parameters->magnetism_distance);
        target->magnetism_level = magnetism;
        if (magnetism > 0.0f)
        {
            object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, target->object_index)->datum;
            /* +380 is unit_definition.unit.flags — targets with magnetism scaling are units */
            unit_definition *object_def = TAG_GET(unit_definition, object->definition_index);
            if ((object_def->unit.flags & (1u << _unit_is_inconsequential)) != 0)
                target->magnetism_level = ((game_globals_player_control *)global_game_globals->player_control.address)->magnetism_inconsequential_target_scale * magnetism;
        }
    }
    else
    {
        target->autoaim_level = 0.0f;
        target->magnetism_level = 0.0f;
    }

    if (target->autoaim_level > 0.0f)
        return 1;
    if (target->magnetism_level > 0.0f)
        return 1;
    return 0;
}
