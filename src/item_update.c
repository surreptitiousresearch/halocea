/* item_update @0x83758680 — per-frame physics/settling update for a dropped item object (weapon/equipment).
 * Runs only for items in free-physics mode (object flag 0x800 set and no owning slot, +284 == -1). When the
 * item's stored scale drifts from 1.0 it re-orthonormalizes the object's forward axis against world up. Moving
 * items (object flag 0x20 clear) integrate velocity (with gravity unless the item definition disables it),
 * sweep the motion segment with collision_test_line, and on a hit spawn material/impact-sound effects, bounce or
 * settle onto the surface (recording the resting surface/BSP or the object it rests on), then translate. Settled
 * items track whether their resting surface broke or their supporting object vanished and fall again if so, while
 * damping angular velocity. A spinning flag (physics +500 bit 4) rotates the orientation about the stored spin
 * axis each tick. Finally the despawn timer is decremented (spawning the definition's detonation effect and
 * deleting the object at zero) and the last-touched game time is stamped when required.
 *
 * The 0xF0-byte stack scratch buffer that collision_test_line fills as a collision_result and
 * object_get_marker_by_name fills as an object_marker is the same slot in the binary; its live ranges never
 * overlap, so it is modeled here as two locals (collision / marker). The decompiler rendered collision_result
 * fields through an object_marker real_matrix4x3 view (node_matrix.n[r][c] etc.); those are mapped back to the
 * real collision_result fields below. Shared _object_datum fields are reached through `od`; the item-specific
 * physics payload past offset 500 (rest surface/BSP, spin axis, timers) is still accessed by raw byte offset. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/item_definition_flags.h"
#include "headers/collision_surface_flags.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/item_datum.h"
#include "headers/item_definition.h"
#include "headers/object_marker.h"
#include "headers/collision_result.h"
#include "headers/collision_bsp.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/sound_location.h"
#include "headers/ppc_intrinsics.h"
#include <string.h>
#include "headers/collision_result_type.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"

/* 0x1FF3E9 = projectile-style test plus every object type except weapons/equipment (value-preserving). */
#define item_update_collision_test_flags \
    (_collision_test_for_projectiles_flags \
     | (_collision_test_objects_all_types_flags \
        & ~((1u << _collision_test_objects_weapons_bit) | (1u << _collision_test_objects_equipment_bit))))


#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern float normalize3d(real_vector3d *v);
extern uint8_t collision_test_line(unsigned int flags, const real_point3d *point0, const real_point3d *point1, int ignore_object_index, collision_result *collision);
extern uint8_t collision_test_point(unsigned int flags, const real_point3d *point, int ignore_object_index);
extern uint8_t material_effect_visible(const real_point3d *origin);
extern void material_effect_new(int effects_definition_index, int16_t effect_index, int16_t material_index, const real_point3d *origin, const real_vector3d *normal, const location *location, float scale, uint8_t is_player);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);
extern void item_align_to_normal_and_point(int item_index, const real_vector3d *normal, const real_point3d *position, real_point3d *new_position);
extern void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives);
extern void item_adjust_for_angular_velocity_change(int object_index);
extern uint8_t game_engine_running(void);
extern int16_t game_connection(void);
extern void object_set_garbage(int object_index, uint8_t garbage);
extern int16_t global_structure_bsp_index_get(void);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint8_t breakable_surface_extant(int16_t breakable_surface_index);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void object_align_marker_to_matrix(object_datum *object, const object_marker *child_marker, const real_matrix4x3 *destination_matrix);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_delete(int object_index);
extern int game_time_get(void);

uint8_t item_update(int item_index)
{
    char *object = (char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    _object_datum *od = &((object_datum *)object)->object;
    item_datum *item = (item_datum *)object;
    item_definition *item_def = TAG_GET(item_definition, *(int *)object);

    real_point3d velocity;
    real_point3d candidate;
    real_matrix4x3 work_matrix;
    collision_result collision;
    object_marker marker;

    if ( (od->flags & (1u << _object_connected_to_map_bit)) == 0 || od->parent_object_index != -1 )
        goto despawn_and_touch;

    /* re-orthonormalize the forward axis against world up when the stored scale has drifted from 1.0 */
    if ( (item_def->item.flags & (1u << _item_always_maintains_z_up_bit)) != 0 && __fabs((od->up.k - (float)1.0)) >= 0.000099999997 )
    {
        od->up = *global_up3d;
        float up_y = od->up.j;
        float up_x = od->up.i;
        float cross_xy = ((od->up.i * od->forward.j)
                               - (od->forward.i * od->up.j));
        float cross_zy = ((od->forward.k * od->up.j)
                               - (od->forward.j * od->up.k));
        float term_up_z = (od->up.k
                               * ((od->forward.k * od->up.j)
                                       - (od->forward.j * od->up.k)));
        float term_up_x = (od->up.i
                               * ((od->forward.i * od->up.k)
                                       - (od->up.i * od->forward.k)));
        od->forward.i = (od->up.k
                                         * ((od->forward.i * od->up.k)
                                                 - (od->up.i * od->forward.k)))
                                 - (od->up.j
                                         * ((od->up.i * od->forward.j)
                                                 - (od->forward.i * od->up.j)));
        od->forward.j = (up_x * cross_xy) - term_up_z;
        od->forward.k = (up_y * cross_zy) - term_up_x;
        if ( normalize3d(&od->forward) == 0.0 )
        {
            od->forward = *global_forward3d;    /* DEVIATION: decompiler split this 3-word vector copy into int-punned word moves */
        }
    }

    unsigned int definition_flags = item_def->item.flags;
    if ( (od->flags & (1u << _object_at_rest_bit)) == 0 )
    {
        /* moving item: integrate velocity (plus gravity unless disabled) and sweep for collisions */
        float velocity_y = od->translational_velocity.j;
        float velocity_z = od->translational_velocity.k;
        velocity.n[0] = od->translational_velocity.i;
        velocity.n[1] = velocity_y;
        velocity.n[2] = velocity_z;

        /* DEVIATION: integrated_z/surface_normal_z/impact_scale respelled double->float 2026-08-18;
           the path is all-single in the binary (fsqrts @0x837588E8, fmuls 10.0f @0x837588EC,
           fcmpu vs 0.0f/1.0f, lfs-only loads @0x837588B4/0x83758950) -- no fdiv/frsp/lfd. */
        float integrated_z;
        if ( (definition_flags & (1u << _item_antigravity_bit)) != 0 )
        {
            integrated_z = velocity.n[2];
        }
        else
        {
            integrated_z = (velocity.n[2] - global_gravity);
            velocity.n[2] = velocity.n[2] - global_gravity;
        }
        candidate.n[0] = od->position.x + velocity.n[0];
        candidate.n[1] = od->position.y + velocity.n[1];
        candidate.n[2] = od->position.z + integrated_z;

        if ( collision_test_line(
                 item_update_collision_test_flags,
                 (const real_point3d *)&od->position,
                 &candidate,
                 item->item.ignore_object_index,
                 &collision) )
        {
            float surface_normal_z = collision.plane.normal.n[2];
            candidate.n[0] = (collision.plane.normal.n[0] * (float)0.050000001) + candidate.n[0];
            candidate.n[1] = (collision.plane.normal.n[1] * (float)0.050000001) + candidate.n[1];
            candidate.n[2] = (collision.plane.normal.n[2] * (float)0.050000001) + candidate.n[2];

            float impact_scale = (__fsqrts(((integrated_z * integrated_z)
                                                                + ((velocity.n[1] * velocity.n[1])
                                                                        + (velocity.n[0] * velocity.n[0]))))
                                        * 10.0f);
            if ( impact_scale >= 0.0f )
            {
                if ( impact_scale > 1.0f )
                    impact_scale = 1.0f;
            }
            else
            {
                impact_scale = 0.0f;
            }

            if ( item_def->item.material_effects.index != -1 )
            {
                if ( material_effect_visible(&collision.point) )
                    /* is_player = 0 (li r10,0); decompiler's SHIWORD(position.w) 3rd arg is the halfword at
                     * buffer+0x34 = collision.material_type, and the 8th arg v20 is a float-slot-skip phantom */
                    material_effect_new(
                        item_def->item.material_effects.index,
                        8,
                        collision.material_type,
                        &collision.point,
                        &collision.plane.normal,
                        &collision.location,
                        impact_scale,
                        0);
                surface_normal_z = collision.plane.normal.n[2];
            }

            int impulse_sound = item_def->item.collision_sound.index;
            if ( impulse_sound != -1 )
            {
                /* DEVIATION: the decompiler rendered these four plain copies through the work_matrix
                 * stack slot with fused 64-bit ld/std temps ("local variable allocation has failed"
                 * residue); disasm 0x83758960-0x837589C0 is word-for-word this struct build:
                 * position = candidate, forward = surface normal, velocity = zero, game_location =
                 * od->location (the 8-byte std from object+0x98). */
                sound_location impact_location;
                impact_location.position = candidate;
                impact_location.forward = collision.plane.normal;
                impact_location.translational_velocity = *global_zero_vector3d;
                impact_location.game_location = od->location;
                /* is_player = 0 (li r6,0); decompiler's LOBYTE(up.k) 4th arg is the reserved GPR slot the
                 * float scale skips over */
                unattached_impulse_sound_new(impulse_sound, &impact_location, impact_scale, 0);
                surface_normal_z = collision.plane.normal.n[2];
            }

            if ( (collision.type == collision_result_structure
               || collision.type == collision_result_object
               && ((1 << DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, collision.object_index)->type)
                 & (object_mask_scenery | object_mask_device)) != 0)
              && surface_normal_z > 0.70709997 )
            {
                if ( -(((float)surface_normal_z * (float)integrated_z)
                                   + ((collision.plane.normal.n[1] * velocity.n[1])
                                           + (collision.plane.normal.n[0] * velocity.n[0]))) < 0.050000001 )
                {
                    /* settle: snap onto the surface and record the resting surface or object */
                    candidate = collision.point;
                    item_align_to_normal_and_point(
                        item_index,
                        &collision.plane.normal,
                        &collision.point,
                        &candidate);
                    double normal_z = collision.plane.normal.n[2];
                    double angular_z = (od->angular_velocity.k * collision.plane.normal.n[2]);
                    double angular_y = od->angular_velocity.j;
                    double surface_normal_y = collision.plane.normal.n[1];
                    double angular_x = od->angular_velocity.i;
                    velocity.n[2] = 0.0;
                    velocity.n[1] = 0.0;
                    velocity.n[0] = 0.0;
                    double angular_dot = (((float)angular_x * collision.plane.normal.n[0])
                                               + ((collision.plane.normal.n[1] * (float)angular_y)
                                                       + (float)angular_z));
                    od->angular_velocity.i = collision.plane.normal.n[0]
                                             * (((float)angular_x * collision.plane.normal.n[0])
                                                     + ((collision.plane.normal.n[1] * (float)angular_y)
                                                             + (float)angular_z));
                    od->angular_velocity.j = (float)surface_normal_y * (float)angular_dot;
                    od->angular_velocity.k = (float)angular_dot * (float)normal_z;
                    if ( !game_engine_running() && od->owner_player_index == -1 )
                        object_set_garbage(item_index, 1u);
                    int physics_flags = item->item.flags;
                    uint8_t rests_on_surface = collision.type == collision_result_structure;
                    od->flags |= (1u << _object_at_rest_bit);
                    if ( rests_on_surface )
                    {
                        int16_t surface_index = (int16_t)collision.surface_index;
                        item->item.flags = physics_flags | (1u << _item_on_structure_bit);
                        item->item.rested_surface_index = surface_index;
                        item->item.bsp_index = global_structure_bsp_index_get();
                    }
                    else
                    {
                        int rest_object_index = collision.object_index;
                        item->item.flags = physics_flags | (1u << _item_on_object_bit);
                        item->item.item_on_rest_object_index = rest_object_index;
                        real_matrix4x3 *rest_node_matrix = object_get_node_matrix(rest_object_index, 0);
                        matrix4x3_inverse_transform_point(
                            rest_node_matrix,
                            &collision.point,
                            &item->item.item_rest_object_offset);
                    }
                    float rest_normal_y = collision.plane.normal.n[1];
                    float rest_normal_z = collision.plane.normal.n[2];
                    item->item.rotation_axis.n[0] = collision.plane.normal.n[0];
                    item->item.rotation_axis.n[1] = rest_normal_y;
                    item->item.rotation_axis.n[2] = rest_normal_z;
                    item_adjust_for_angular_velocity_change(item_index);
                    item->item.ignore_object_index = -1;
                    goto post_move;
                }
            }

            /* bounce: reflect velocity about the surface normal */
            double bounce_normal_x = collision.plane.normal.n[0];
            double bounce_normal_y = collision.plane.normal.n[1];
            double reflection = -((((float)surface_normal_z * (float)integrated_z) * (float)1.4)
                                               - ((((float)bounce_normal_x * velocity.n[0]) * (float)-1.4)
                                                       - (((float)bounce_normal_y * velocity.n[1]) * (float)1.4)));
            if ( collision.type != collision_result_structure && reflection >= 1.5 )
                reflection = 1.5;
            velocity.n[0] = ((float)bounce_normal_x * (float)reflection) + velocity.n[0];
            velocity.n[1] = ((float)bounce_normal_y * (float)reflection) + velocity.n[1];
            candidate = collision.point;
            velocity.n[2] = ((float)reflection * (float)surface_normal_z) + (float)integrated_z;
            if ( collision_test_point(item_update_collision_test_flags, &candidate, item_index) )
            {
                candidate.n[0] = (collision.plane.normal.n[0] * (float)0.050000001) + collision.point.n[0];
                candidate.n[1] = (collision.plane.normal.n[1] * (float)0.050000001) + collision.point.n[1];
                candidate.n[2] = (collision.plane.normal.n[2] * (float)0.050000001) + collision.point.n[2];
            }
            collision_test_point(item_update_collision_test_flags, &candidate, item_index);
        }

post_move:
        od->translational_velocity.i = velocity.n[0];
        od->translational_velocity.j = velocity.n[1];
        od->translational_velocity.k = velocity.n[2];
        object_translate(item_index, &candidate, &collision.location);
        goto spin;
    }

    if ( (definition_flags & (1u << _item_antigravity_bit)) == 0 )
    {
        /* settled item: verify its resting surface / supporting object is still valid, else fall */
        object_get_marker_by_name(item_index, "ground point", &marker, 1);
        if ( (item->item.flags & (1u << _item_on_structure_bit)) != 0
          && (uint16_t)item->item.rested_surface_index != 0xFFFF
          && item->item.bsp_index == global_structure_bsp_index_get() )
        {
            collision_surface *rest_surface =
                (collision_surface *)global_collision_bsp->surfaces.address + item->item.rested_surface_index;
            if ( (rest_surface->flags & (1u << _collision_surface_breakable_bit)) == 0 || breakable_surface_extant(rest_surface->breakable_surface_index) )
                goto damp_angular;
            item->item.flags &= ~(1u << _item_on_structure_bit);
            item->item.rested_surface_index = -1;
            goto fall;
        }
        if ( (item->item.flags & (1u << _item_on_object_bit)) != 0 )
        {
            if ( !object_try_and_get_and_verify_type(item->item.item_on_rest_object_index, object_mask_all) )
            {
                item->item.flags &= ~(1u << _item_on_object_bit);
fall:
                velocity.n[0] = global_down3d->n[0] * global_gravity;
                velocity.n[1] = global_down3d->n[1] * global_gravity;
                velocity.n[2] = global_down3d->n[2] * global_gravity;
                item_accelerate(item_index, (const real_vector3d *)&velocity, 0);
                goto damp_angular;
            }
            real_matrix4x3 *support_matrix = object_get_node_matrix(item->item.item_on_rest_object_index, 0);
            matrix4x3_transform_point(support_matrix, &item->item.item_rest_object_offset, &velocity);
            item_align_to_normal_and_point(item_index, &item->item.rotation_axis, &velocity, NULL);
        }
damp_angular:
        od->angular_velocity.i = od->angular_velocity.i * (float)0.89999998;
        od->angular_velocity.j = od->angular_velocity.j * (float)0.89999998;
        od->angular_velocity.k = od->angular_velocity.k * (float)0.89999998;
        item_adjust_for_angular_velocity_change(item_index);
    }

spin:
    if ( (item->item.flags & (1u << _item_has_nonzero_angular_velocity_bit)) != 0 )
    {
        char use_object_axes;
        if ( game_connection() && game_connection() != _game_connection_film_playback
          || (od->flags & (1u << _object_at_rest_bit)) == 0
          || (use_object_axes = 0, !object_get_marker_by_name(item_index, "ground point", &marker, 1)) )
        {
            use_object_axes = 1;
        }
        if ( use_object_axes )
        {
            rotate_vector_about_axis(
                &od->forward,
                &item->item.rotation_axis,
                item->item.rotation_sine,
                item->item.rotation_cosine);
            rotate_vector_about_axis(
                &od->up,
                &item->item.rotation_axis,
                item->item.rotation_sine,
                item->item.rotation_cosine);
        }
        else
        {
            memcpy(&work_matrix, &marker.matrix, sizeof(work_matrix));
            rotate_vector_about_axis(
                &work_matrix.forward,
                &item->item.rotation_axis,
                item->item.rotation_sine,
                item->item.rotation_cosine);
            rotate_vector_about_axis(&work_matrix.up, &item->item.rotation_axis,
                                     item->item.rotation_sine, item->item.rotation_cosine);
            work_matrix.n[1][0] = (work_matrix.n[0][2] * work_matrix.n[2][1]) - (work_matrix.n[2][2] * work_matrix.n[0][1]);
            work_matrix.n[1][1] = (work_matrix.n[2][2] * work_matrix.n[0][0]) - (work_matrix.n[0][2] * work_matrix.n[2][0]);
            work_matrix.n[1][2] = (work_matrix.n[0][1] * work_matrix.n[2][0]) - (work_matrix.n[2][1] * work_matrix.n[0][0]);
            work_matrix.n[0][1] = (work_matrix.n[1][2] * work_matrix.n[2][0]) - (work_matrix.n[2][2] * work_matrix.n[1][0]);
            work_matrix.n[0][2] = (work_matrix.n[2][1] * work_matrix.n[1][0]) - (work_matrix.n[1][1] * work_matrix.n[2][0]);
            work_matrix.n[0][0] = (work_matrix.n[1][1] * work_matrix.n[2][2]) - (work_matrix.n[1][2] * work_matrix.n[2][1]);
            normalize3d(&work_matrix.forward);
            normalize3d(&work_matrix.left);
            normalize3d(&work_matrix.up);
            object_align_marker_to_matrix((object_datum *)object, &marker, &work_matrix);
        }
        normalize3d(&od->up);

        /* re-orthonormalize the forward axis against the (now-rotated) up axis */
        float up_y = od->up.j;
        float up_x = od->up.i;
        float cross_yx = ((od->forward.j * od->up.i)
                               - (od->up.j * od->forward.i));
        float cross_zy = ((od->forward.k * od->up.j)
                               - (od->forward.j * od->up.k));
        float term_up_x = (od->up.i
                               * ((od->up.k * od->forward.i)
                                       - (od->forward.k * od->up.i)));
        float term_up_z = (od->up.k
                               * ((od->forward.k * od->up.j)
                                       - (od->forward.j * od->up.k)));
        od->forward.i = (od->up.k
                                         * ((od->up.k * od->forward.i)
                                                 - (od->forward.k * od->up.i)))
                                 - (od->up.j
                                         * ((od->forward.j * od->up.i)
                                                 - (od->up.j * od->forward.i)));
        od->forward.k = (up_y * cross_zy) - term_up_x;
        od->forward.j = (up_x * cross_yx) - term_up_z;
        normalize3d(&od->forward);
    }

despawn_and_touch:;
    int16_t despawn_timer = item->item.detonation_ticks;
    if ( despawn_timer > 0 )
    {
        int16_t remaining = despawn_timer - 1;
        item->item.detonation_ticks = remaining;
        if ( !remaining )
        {
            /* color/impulse_field args (r9/r10) are both 0 here: r9 = li 0, r10 only reaches this branch when the
             * timer hit 0; decompiler's v2/v1 are stale-register phantoms */
            effect_new_from_object(item_def->item.detonation_effect.index, item_index, item_index, -1, 0.0, 0.0, NULL, NULL);
            object_delete(item_index);
        }
    }
    if ( (item->item.flags & (1u << _item_in_unit_inventory_bit)) != 0 )
        item->item.last_owned_time = game_time_get();
    return 1;
}
