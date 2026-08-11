/* contrails_update @0x8373FDA8 — advances every live contrail this frame. For each contrail attached
 * to an object, the object's density function drives whether the contrail is emitting; emission edges
 * inject a point, and while emitting new points are spawned at the computed rate. Independently of
 * attachment, the contrail's frame timer advances (spawning new frames at the definition's period,
 * optionally scaled by density), and its texture scroll offsets are integrated. Finally point geometry
 * is updated and a contrail with no remaining points and no owning object is deleted.
 * (scale_flags: 0x20 density-scales-frame-rate, 0x100 density-scales-u-scroll, 0x200 density-scales-v-scroll) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/contrail_active_flags.h"
#include "headers/contrail_definition_scaleable_values_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void *datum_get(data_array *data, int index);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void contrail_add_points(int contrail_index, int16_t contrail_point_count, uint8_t force);
extern int16_t contrail_compute_new_point_count(int contrail_index, float dt);
extern void contrail_next_frame(contrail_datum *contrail);
extern void contrail_update_points(int contrail_index, float dt);
extern void contrail_delete(int contrail_index);

void contrails_update(float dt)
{
    int i;

    for ( i = data_next_index(contrail_data, -1); i != -1; i = data_next_index(contrail_data, i) )
    {
        contrail_datum *contrail = datum_get(contrail_data, i);
        int object_index = contrail->object_index;
        float frame_dt = dt - contrail->expired_dt;
        contrail_definition *definition = TAG_GET(contrail_definition, contrail->definition_index);
        float period;

        contrail->expired_dt = 0.0f;

        if ( object_index != -1 )
        {
            int emitting = object_get_function_value(object_index, contrail->density_function_index, &contrail->density);
            if ( emitting != (contrail->flags & (1u << _contrail_active_bit)) )
            {
                /* emission edge: inject a single point at zero density, then restore density */
                float saved_density = contrail->density;
                contrail->density = 0.0f;
                contrail_add_points(i, 1, 1u);
                contrail->density = saved_density;
            }
            if ( emitting )
                contrail->flags |= (1u << _contrail_active_bit);
            else
                contrail->flags &= ~(1u << _contrail_active_bit);
            if ( emitting )
            {
                int16_t new_points = contrail_compute_new_point_count(i, dt);
                contrail_add_points(i, new_points, 1u);
            }
        }

        /* advance the frame timer (period optionally scaled by density) */
        period = definition->frames_per_second;
        if ( (definition->scale_flags & (1u << _contrail_animation_rate_scales_bit)) != 0 )
            period = contrail->density * definition->frames_per_second;
        {
            float remaining = frame_dt;
            float frame_period = 1.0f / period;
            if ( frame_dt > 0.0f )
            {
                while ( 1 )
                {
                    float time_to_next = frame_period - contrail->frame_time;
                    if ( time_to_next > remaining )
                    {
                        contrail->frame_time += remaining;
                        break;
                    }
                    contrail_next_frame(contrail);
                    remaining -= time_to_next;
                    if ( remaining <= 0.0f )
                        break;
                }
            }
        }

        /* integrate texture scroll (u and v rates optionally scaled by density) */
        {
            float u_rate = definition->texture_animation_u;
            float v_rate;
            if ( (definition->scale_flags & (1u << _contrail_texture_animation_u_bit)) != 0 )
                u_rate = contrail->density * definition->texture_animation_u;
            contrail->texture_offset_u = -((u_rate * frame_dt) - contrail->texture_offset_u);

            v_rate = definition->texture_animation_v;
            if ( (definition->scale_flags & (1u << _contrail_texture_animation_v_bit)) != 0 )
                v_rate = contrail->density * definition->texture_animation_v;
            contrail->texture_offset_v = (v_rate * frame_dt) + contrail->texture_offset_v;
        }

        contrail_update_points(i, dt);

        {
            int j;
            for ( j = 0; j < 4; j = (int16_t)(j + 1) )
            {
                if ( contrail->first_contrail_point_indices[j] != -1 )
                    break;
            }
            if ( j == 4 && contrail->object_index == -1 )
                contrail_delete(i);
        }
    }
}
