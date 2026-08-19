/* biped_get_sight_position @0x837AC820 — compute the world point a biped sights/aims from. Starts at the
 * biped's origin (or an estimated body position), then adjusts for eye height. For estimate_mode 3 the
 * desired gun offset is rotated into the desired-facing frame and added. Otherwise the eye height is
 * interpolated between biped.standing_camera_height and biped.crouching_camera_height by a crouch fraction: 0
 * for mode 1, 1 for mode 2, else the live biped.crouch advanced by biped.runtime_crouch_transition_velocity
 * over the time since the last tick.
 *
 * The biped variant definition floats are read by index off the tag data; the biped datum uses byte
 * offsets. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum_flags.h"
#include "headers/base_seat.h"
#include "headers/unit_estimate_position_type.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float game_time_get_since_tick(void);

void biped_get_sight_position(int biped_index, int16_t estimate_mode, const real_point3d *estimated_body_position,
                              real_vector3d *desired_facing, const real_vector3d *desired_gun_offset,
                              real_point3d *sight_position)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *variant = TAG_GET(biped_definition, biped->definition_index);

    if (estimate_mode)
        *sight_position = *estimated_body_position;
    else
        object_get_origin(biped_index, sight_position);

    if (estimate_mode == _unit_estimate_gun_position)
    {
        float gun_x = desired_gun_offset->n[0];
        float facing_x = desired_facing->n[0];
        float offset_x = (desired_facing->n[0] * desired_gun_offset->n[0]) + sight_position->n[0];
        float origin_y = sight_position->n[1];
        float neg_facing_y = -desired_facing->n[1];
        sight_position->n[0] = (desired_facing->n[0] * desired_gun_offset->n[0]) + sight_position->n[0];
        float origin_z = sight_position->n[2];
        float offset_y = (desired_facing->n[1] * gun_x) + origin_y;
        sight_position->n[1] = (desired_facing->n[1] * gun_x) + origin_y;
        float offset_z = (desired_facing->n[2] * gun_x) + origin_z;
        sight_position->n[2] = (desired_facing->n[2] * gun_x) + origin_z;
        float gun_y = desired_gun_offset->n[1];
        float result_z = ((desired_gun_offset->n[1] * 0.0f) + offset_z);
        float result_x = ((desired_gun_offset->n[1] * neg_facing_y) + offset_x);
        sight_position->n[2] = ((desired_gun_offset->n[1] * 0.0f) + offset_z);
        sight_position->n[0] = result_x;
        sight_position->n[1] = ((gun_y * facing_x) + offset_y);
        sight_position->n[2] = desired_gun_offset->n[2] + result_z;
    }
    else
    {
        float crouch;
        if (estimate_mode == _unit_estimate_head_standing)
        {
            crouch = 0.0f;
        }
        else if (estimate_mode == _unit_estimate_head_crouching)
        {
            crouch = 1.0f;
        }
        else
        {
            crouch = biped->biped.crouch;
            if ((biped->biped.flags & (1u << _biped_airborne_bit)) == 0 && crouch > 0.0f && crouch < 1.0f)
            {
                float since_tick = game_time_get_since_tick();
                if (biped->unit.animation.base_seat_index == _base_seat_crouch)
                    crouch = ((variant->biped.runtime_crouch_transition_velocity * (since_tick * 29.999998f)) + crouch);
                else
                    crouch = -((variant->biped.runtime_crouch_transition_velocity * (since_tick * 29.999998f)) - crouch);
            }
        }
        sight_position->n[2] = (((1.0f - crouch) * variant->biped.standing_camera_height)
                                     + (variant->biped.crouching_camera_height * crouch)) + sight_position->n[2];
    }
}
