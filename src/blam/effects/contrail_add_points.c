/* contrail_add_points @0x8373F348 — spawn new contrail points at the emitting object's markers.
 * Resolves up to 4 markers by name on the object, and for each marker whose transform has changed
 * since the last point (or when forced), prepends contrail_point_count new points to that marker's
 * point list, interpolating position/velocity/density between the new point and the previous head so a
 * smooth segment is laid down across the frame.
 *
 * Types: contrail_datum, contrail_point_datum, contrail_definition (all DB-verified). The
 * definition's scale_flags bits 0x8/0x10 gate the density-scaled axial/perpendicular velocity terms. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_point_datum.h"
#include "headers/contrail_definition.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/model.h"
#include "headers/contrail_point_datum_flags.h"
#include "headers/contrail_definition_scaleable_values_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern float contrail_scale_random_value(float density, float lower_bound, float upper_bound, unsigned int scale_flags, int16_t first_bit_index);
extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void scenario_location_from_line(location *out_location, const location *start_location, const real_point3d *start_point, const real_point3d *end_point);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern int datum_new(data_array *data);

void contrail_add_points(int contrail_index, int16_t contrail_point_count, uint8_t force)
{
    contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
    contrail_definition *definition =
        TAG_GET(contrail_definition, contrail->definition_index);
    int object_index = contrail->object_index;
    object_marker markers[4];
    int16_t marker_count;
    int marker;

    if ( !contrail_point_count )
        return;

    /* resolve the marker name from the object's render model marker table */
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        _object_definition *object_def =
            TAG_GET(_object_definition, object->definition_index);
        const char *marker_name =
            ((object_attachment_definition *)object_def->attachments.address)[contrail->attachment_index].marker_name;
        marker_count = object_get_marker_by_name(object_index, marker_name, markers, 4);
    }

    if ( marker_count <= 0 )
        return;

    {
        int flags = definition->scale_flags;
        float density = contrail->density;
        /* args corrected against disasm: first_bit_index = 1 (the decompiler mislabeled these as the
         * marker count and an uninitialized local). */
        float spawn_scale = contrail_scale_random_value(density, definition->point_velocity_lower_bound,
                                                        definition->point_velocity_upper_bound, flags, 1);
        float axial_velocity = definition->point_velocity_cone_angle;
        float perpendicular_velocity = definition->point_inherited_velocity_fraction;

        if ( (flags & (1u << _contrail_point_velocity_cone_angle_bit)) != 0 )
            axial_velocity = density * definition->point_velocity_cone_angle;
        if ( (flags & (1u << _contrail_point_velocity_fraction_bit)) != 0 )
            perpendicular_velocity = density * definition->point_inherited_velocity_fraction;

        for ( marker = 0; marker < marker_count; marker = (int16_t)(marker + 1) )
        {
            object_marker *marker_data = &markers[marker];
            int16_t points_to_add = contrail_point_count;
            contrail_point_datum *previous_point = DATA_ARRAY_ELEMENT(contrail_point_data,
                    contrail_point_datum, contrail->first_contrail_point_indices[marker]);
            int chain_index;

            if ( contrail->first_contrail_point_indices[marker] == -1 )
            {
                /* empty list: lay a single fresh point */
                previous_point = NULL;
                points_to_add = 1;
            }
            else
            {
                /* coarse compare of the marker's translation against the head point's stored position;
                 * if unchanged (and not forced), skip this marker. */
                const unsigned char *marker_pos = (const unsigned char *)&marker_data->matrix.n[3];
                const unsigned char *stored_pos = (const unsigned char *)&previous_point->position;
                int difference = 0;
                while ( marker_pos != (const unsigned char *)&marker_data[1] )
                {
                    difference = *marker_pos - *stored_pos;
                    if ( *marker_pos != *stored_pos )
                        break;
                    ++marker_pos;
                    ++stored_pos;
                }
                if ( !difference && !force )
                    continue;
            }

            for ( chain_index = 1; chain_index <= points_to_add; chain_index = (int16_t)(chain_index + 1) )
            {
                int new_index = datum_new(contrail_point_data);
                contrail_point_datum *point;
                real_vector3d cone_direction;
                real_vector3d object_velocity;

                if ( new_index == -1 )
                    continue;

                point = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, new_index);
                point->time = 0.0f;
                point->flags = (1u << _contrail_point_new_bit) | (1u << _contrail_point_transitioning_bit);
                point->delta = 0.0f;
                point->state_index = -1;
                point->density = contrail->density;

                seed_random_vector_in_cone3d(get_global_local_random_seed_address(),
                        (const real_vector3d *)&marker_data->matrix.n[0], 0.0f, axial_velocity, &cone_direction);

                point->position.n[0] = marker_data->matrix.n[3][0];
                point->position.n[1] = marker_data->matrix.n[3][1];
                point->position.n[2] = marker_data->matrix.n[3][2];
                scenario_location_from_point(&point->location, &point->position);

                object_get_velocities(contrail->object_index, &object_velocity, NULL);
                point->velocity.n[0] = (perpendicular_velocity * object_velocity.n[0]) + (cone_direction.n[0] * spawn_scale);
                point->velocity.n[1] = (perpendicular_velocity * object_velocity.n[1]) + (cone_direction.n[1] * spawn_scale);
                point->velocity.n[2] = (perpendicular_velocity * object_velocity.n[2]) + (cone_direction.n[2] * spawn_scale);

                if ( chain_index < points_to_add )
                {
                    /* interpolate this point against the previous head along the segment */
                    float t = (float)chain_index / (float)points_to_add;
                    float one_minus_t = 1.0f - t;
                    real_point3d interpolated;

                    point->density = (previous_point->density * one_minus_t) + (point->density * t);

                    interpolated.n[0] = (previous_point->position.n[0] * one_minus_t) + (t * point->position.n[0]);
                    interpolated.n[1] = (previous_point->position.n[1] * one_minus_t) + (point->position.n[1] * t);
                    interpolated.n[2] = (previous_point->position.n[2] * one_minus_t) + (point->position.n[2] * t);
                    scenario_location_from_line(&point->location, &point->location,
                            &point->position, &interpolated);

                    point->position.n[1] = interpolated.n[1];
                    point->position.n[0] = interpolated.n[0];
                    point->position.n[2] = interpolated.n[2];

                    point->velocity.n[0] = (previous_point->velocity.n[0] * one_minus_t) + (t * point->velocity.n[0]);
                    point->velocity.n[1] = (previous_point->velocity.n[1] * one_minus_t) + (point->velocity.n[1] * t);
                    point->velocity.n[2] = (previous_point->velocity.n[2] * one_minus_t) + (point->velocity.n[2] * t);
                }

                point->next_contrail_point_index = contrail->first_contrail_point_indices[marker];   /* link */
                ++contrail->contrail_point_counts[marker];
                contrail->first_contrail_point_indices[marker] = new_index;
            }
        }
    }
}
