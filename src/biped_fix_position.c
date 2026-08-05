/* biped_fix_position @0x837ACCC0 — find a nearby unobstructed position for a biped's physics pill near
 * `new_position` (or its current position), trying a fixed set of fudge offsets in order. For each candidate it
 * requires: a valid scenario cluster, collision_fix_pill success, no pill collision, and — when a line-of-site
 * object is given — that the pill does not hit that object's collision model and that the candidate has clear
 * line-of-site to/from it. On the first candidate that passes it optionally commits the biped to that position
 * (object position write + node matrix recompute + object_translate) unless dont_teleport is set, writes
 * final_position, and returns 1. Returns 0 if no candidate works.
 *
 * Deviation: the Hex-Rays output is float-GPR-skip scrambled — it invents a phantom 9th parameter `a9` and shifts
 * the three trailing bool names. By register (DB prototype + prologue): r8=fix_below_new_position (selects the
 * object-basis fudge transform and the 27/18 fudge count), r9=dont_teleport (gates the position commit),
 * r10=use_radius_as_multiplier (scales the fudge factor by pill width). Several call arguments the decompiler
 * rendered as garbage (collision result pointers, ignore indices) are corrected from the call-site disassembly. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/collision_result.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_model_test_pill_result.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"

#include "headers/global_tag_instances.h"  /* global_tag_instances; base_address@0x14 (tag-instance table) */
#include "headers/biped_definition_flags.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"
extern const real_vector3d fudge_vectors[];

extern float __fsqrts(float);
extern float __fabs(float);
extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern int16_t scenario_cluster_index_from_point(const real_point3d *point);
extern uint8_t collision_fix_pill(unsigned int flags, const real_point3d *old_position, float distance, float height, float width, int ignore_object_index, real_point3d *new_position);
extern uint8_t collision_test_pill(unsigned int flags, const real_point3d *point, const real_vector3d *vector, float radius, int ignore_object_index, collision_result *collision);
extern uint8_t collision_model_test_pill(const collision_model_instance *instance, const real_point3d *point, const real_vector3d *vector, float radius, collision_model_test_pill_result *result);
extern uint8_t collision_test_line(unsigned int flags, const real_point3d *point0, const real_point3d *point1, int ignore_object_index, collision_result *collision);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void object_compute_node_matrices_recursive(int object_index);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);

static biped_datum *object_data_from_index(int object_index)
{
    return ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
}

static const biped_definition *tag_data_from_object(const biped_datum *object_data)
{
    return TAG_GET(const biped_definition, object_data->definition_index);
}

uint8_t biped_fix_position(int biped_index, int line_of_site_object_index, const real_point3d *new_position,
                                   real_point3d *final_position, float max_radius_fudge_factor,
                                   uint8_t fix_below_new_position, uint8_t dont_teleport,
                                   uint8_t use_radius_as_multiplier)
{
    int found = 0;

    if ( biped_index == -1 && line_of_site_object_index == -1 )
        return found;

    real_point3d los_position;
    if ( line_of_site_object_index != -1 )
    {
        const biped_datum *los_object = object_data_from_index(line_of_site_object_index);
        los_position.n[0] = los_object->object.bounding_sphere_center.n[0];
        los_position.n[1] = los_object->object.bounding_sphere_center.n[1];
        los_position.n[2] = los_object->object.bounding_sphere_center.n[2];
    }

    int moved_object = biped_index;
    char los_only = 0;
    if ( biped_index == -1 )
    {
        moved_object = line_of_site_object_index;
        los_only = 1;
    }

    biped_datum *object_data = object_data_from_index(moved_object);
    const biped_definition *object_tag = tag_data_from_object(object_data);

    unsigned int flags;
    if ( (object_tag->biped.flags & (1u << _biped_passes_through_bipeds_bit)) != 0 )
        flags = _collision_test_for_bipeds_passthrough_living_flags;
    else
        flags = _collision_test_for_bipeds_living_flags;

    /* obtain the pill (base/height/width); base comes from new_position when supplied */
    real_point3d base;
    float pill_height;
    float pill_width;
    if ( new_position )
    {
        real_point3d discarded_pill_base;
        base = *new_position;
        biped_get_physics_pill(moved_object, &discarded_pill_base, &pill_height, &pill_width);
    }
    else
    {
        biped_get_physics_pill(moved_object, &base, &pill_height, &pill_width);
    }

    if ( los_only )
        moved_object = -1;

    collision_model_instance los_instance;
    if ( line_of_site_object_index != -1 )
        collision_model_instance_new(&los_instance, line_of_site_object_index);

    /* object up axis (normalized cross of the forward/up basis columns; [0..2]=forward, [3..5]=up) */
    const float *basis = (const float *)&object_data->object.forward;
    float up_x = ((basis[5] * basis[1]) - (basis[4] * basis[2]));
    float up_z = ((basis[0] * basis[4]) - (basis[3] * basis[1]));
    float up_y = ((basis[3] * basis[2]) - (basis[0] * basis[5]));
    float up_length = __fsqrts(((up_y * up_y) + ((up_z * up_z) + (up_x * up_x))));
    if ( __fabs(up_length) >= 0.000099999997 )
    {
        float inverse = ((float)1.0 / up_length);
        up_x = (inverse * up_x);
        up_y = (inverse * up_y);
        up_z = (inverse * up_z);
    }

    real_vector3d pill_vector;
    pill_vector.n[0] = (global_up3d->n[0] * pill_height);
    pill_vector.n[1] = (global_up3d->n[1] * pill_height);
    pill_vector.n[2] = (global_up3d->n[2] * pill_height);

    if ( use_radius_as_multiplier )
        max_radius_fudge_factor = (pill_width * max_radius_fudge_factor);

    int fudge_count = fix_below_new_position ? 27 : 18;

    int16_t i = 0;
    do
    {
        int16_t current = i;
        if ( current >= fudge_count )
            break;

        real_point3d candidate;
        if ( fix_below_new_position )
        {
            float a = (fudge_vectors[current].n[0] * max_radius_fudge_factor);
            float b = (fudge_vectors[current].n[1] * max_radius_fudge_factor);
            float c = (fudge_vectors[current].n[2] * max_radius_fudge_factor);
            candidate.n[0] = ((a * basis[0]) + base.n[0]);
            candidate.n[1] = ((a * basis[1]) + base.n[1]);
            float z_partial = ((up_z * b) + ((a * basis[2]) + base.n[2]));
            candidate.n[0] = ((b * up_x) + candidate.n[0]);
            candidate.n[1] = ((b * up_y) + candidate.n[1]);
            candidate.n[2] = z_partial;
            candidate.n[0] = ((basis[3] * c) + candidate.n[0]);
            candidate.n[1] = ((basis[4] * c) + candidate.n[1]);
            candidate.n[2] = ((basis[5] * c) + z_partial);
        }
        else
        {
            candidate.n[0] = ((fudge_vectors[current].n[0] * max_radius_fudge_factor) + base.n[0]);
            candidate.n[1] = ((fudge_vectors[current].n[1] * max_radius_fudge_factor) + base.n[1]);
            candidate.n[2] = ((fudge_vectors[current].n[2] * max_radius_fudge_factor) + base.n[2]);
        }

        collision_result pill_collision;
        collision_model_test_pill_result model_pill_result;
        collision_result line_collision;
        if ( scenario_cluster_index_from_point(&candidate) != -1
          && collision_fix_pill(flags, &candidate, (float)(pill_width * 2.0), pill_height, pill_width,
                                moved_object, &candidate)
          && !collision_test_pill(flags, &candidate, &pill_vector, pill_width, moved_object, &pill_collision)
          && (line_of_site_object_index == -1
           || !collision_model_test_pill(&los_instance, &candidate, &pill_vector, pill_width, &model_pill_result)
           && (!collision_test_line(flags, &candidate, &los_position, moved_object, &line_collision)
               || line_collision.object_index == line_of_site_object_index)
           && (!collision_test_line(flags, &los_position, &candidate, line_of_site_object_index, &line_collision)
               || line_collision.object_index == moved_object)) )
        {
            const biped_definition *moved_tag = tag_data_from_object(object_data);
            location candidate_location;
            scenario_location_from_point(&candidate_location, &candidate);
            if ( (moved_tag->biped.flags & (1u << _biped_pill_centered_at_origin_bit)) == 0 )
                candidate.n[2] = (candidate.n[2] - moved_tag->biped.collision_radius);

            if ( moved_object != -1 && !dont_teleport )
            {
                object_data->object.position.n[0] = candidate.n[0];
                object_data->object.position.n[1] = candidate.n[1];
                object_data->object.position.n[2] = candidate.n[2];
                object_compute_node_matrices_recursive(moved_object);
                object_translate(moved_object, &candidate, &candidate_location);
            }

            if ( final_position )
                *final_position = candidate;

            found = 1;
        }

        i = (int16_t)(current + 1);
    }
    while ( !(char)found );

    return found;
}
