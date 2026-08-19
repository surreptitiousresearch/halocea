/* item_accelerate @0x83758278 — apply a damage impulse to a free item (weapon/equipment). No-op if the
 * item ignores acceleration (item.flags bit 0x20) or is attached. Optionally detonates explosive items
 * (single-player). If the item is currently resting on the ground (item.flags bit 0x8) and the impulse is
 * significant, it lifts the item ~0.05 above its ground-contact plane and clears the resting state; a
 * negligible impulse just wakes it. Adds the impulse to translational_velocity and an angular kick:
 * for a ground-resting item with a tiny impulse the kick is about the ground normal (or world up) with a
 * random +-pi/2 spin; otherwise about (up x impulse) (random direction if degenerate) scaled by the impulse
 * magnitude (or a random value when the magnitude is tiny). Finally re-solves angular velocity and clears
 * the item's garbage flag. */

#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/item_definition_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/object_marker.h"
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/object_header_datum.h"
#include "headers/item_datum.h"
#include "headers/item_definition.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>
#include "headers/location.h"
extern float __fsqrts(float);

extern uint32_t *get_global_random_seed_address(void);
extern uint8_t game_engine_running(void);
extern float normalize3d(real_vector3d *v);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern void item_detonate(int item_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_plane3d *bsp3d_get_plane_from_designator(const bsp3d *bsp, int plane_designator, real_plane3d *result);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern void item_adjust_for_angular_velocity_change(int object_index);
extern void object_set_garbage(int object_index, uint8_t garbage);

void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives)
{
    item_datum *item = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    item_definition *definition = TAG_GET(item_definition, item->definition_index);
    if ( (item->item.flags & (1u << _item_does_not_accelerate_bit)) != 0 || item->object.parent_object_index != -1 )
        return;

    if ( detonates_explosives && !game_engine_running() && (definition->item.flags & (1u << _item_destroyed_by_explosions_bit)) != 0 )
        item_detonate(item_index);

    float impulse_sq = acceleration->n[2] * acceleration->n[2]
                       + (acceleration->n[0] * acceleration->n[0] + acceleration->n[1] * acceleration->n[1]);

    if ( (item->item.flags & (1u << _item_on_structure_bit)) != 0 )  /* resting on the ground */
    {
        if ( impulse_sq >= 0.000099999997f )
        {
            object_marker marker;
            if ( object_get_marker_by_name(item_index, "ground point", &marker, 1) )
            {
                real_plane3d ground_plane;
                /* dword 3*idx = collision_surface[idx].plane_designator (DB: collision_surface = 12B, .plane_designator int @0) */
                bsp3d_get_plane_from_designator(&global_collision_bsp->bsp3d,
                    ((const collision_surface *)global_collision_bsp->surfaces.address)[item->item.rested_surface_index].plane_designator,
                    &ground_plane);
                /* lift the ground marker 0.05 above the plane */
                float distance_above = 0.050000001f
                    - (((ground_plane.n.n[0] * marker.matrix.n[3][0])
                        + (marker.matrix.n[3][1] * ground_plane.n.n[1]
                           + marker.matrix.n[3][2] * ground_plane.n.n[2])) - ground_plane.d);
                real_point3d new_position;
                new_position.n[0] = ground_plane.n.n[0] * distance_above + marker.matrix.n[3][0];
                new_position.n[1] = ground_plane.n.n[1] * distance_above + marker.matrix.n[3][1];
                new_position.n[2] = ground_plane.n.n[2] * distance_above + marker.matrix.n[3][2];
                object_translate(item_index, &new_position, 0);
            }
            item->object.flags &= ~(1u << _object_at_rest_bit);
            item->item.flags &= ~(1u << _item_on_structure_bit);
        }
    }
    else
    {
        item->object.flags &= ~(1u << _object_at_rest_bit);
    }

    item->object.translational_velocity.n[0] += acceleration->n[0];
    item->object.translational_velocity.n[1] += acceleration->n[1];
    item->object.translational_velocity.n[2] += acceleration->n[2];

    if ( item->item.ignore_object_index == -1 && (item->item.flags & (1u << _item_on_structure_bit)) != 0
      && impulse_sq < 0.000099999997f )
    {
        /* gentle settle: spin about the ground normal (or world up) by a random +-pi/2 */
        real_plane3d settle;
        object_marker marker;
        if ( object_get_marker_by_name(item_index, "ground point", &marker, 1) )
        {
            settle.n.n[0] = marker.matrix.n[2][0];  /* marker up row */
            settle.n.n[1] = marker.matrix.n[2][1];
            settle.n.n[2] = marker.matrix.n[2][2];
        }
        else
        {
            settle.n.n[0] = global_up3d->n[0];
            settle.n.n[1] = global_up3d->n[1];
            settle.n.n[2] = global_up3d->n[2];
        }
        float spin = real_seed_random_range(get_global_random_seed_address(), -1.5707964f, 1.5707964f);
        item->object.angular_velocity.n[0] += settle.n.n[0] * spin;
        item->object.angular_velocity.n[1] += settle.n.n[1] * spin;
        item->object.angular_velocity.n[2] += settle.n.n[2] * spin;
    }
    else
    {
        float magnitude = __fsqrts(acceleration->n[0] * acceleration->n[0]
                                   + (acceleration->n[1] * acceleration->n[1]
                                      + acceleration->n[2] * acceleration->n[2]));
        if ( magnitude < 0.000099999997f )
            magnitude = real_seed_random(get_global_random_seed_address());

        real_vector3d axis;  /* up x impulse */
        axis.n[0] = (global_up3d->n[1] * acceleration->n[2]) - (global_up3d->n[2] * acceleration->n[1]);
        axis.n[1] = (global_up3d->n[2] * acceleration->n[0]) - (global_up3d->n[0] * acceleration->n[2]);
        axis.n[2] = (global_up3d->n[0] * acceleration->n[1]) - (global_up3d->n[1] * acceleration->n[0]);
        if ( normalize3d(&axis) <= 0.0f )
            seed_random_direction3d(get_global_random_seed_address(), &axis);

        float random = real_seed_random(get_global_random_seed_address());
        float spin = random * magnitude * 1.5707964f;
        axis.n[0] *= spin;
        axis.n[1] *= spin;
        axis.n[2] *= spin;
        item->object.angular_velocity.n[0] += axis.n[0];
        item->object.angular_velocity.n[1] += axis.n[1];
        item->object.angular_velocity.n[2] += axis.n[2];
    }

    item_adjust_for_angular_velocity_change(item_index);
    object_set_garbage(item_index, 0);
}
