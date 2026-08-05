/* collision_move_point @0x83774498 — slide a point through the world along a movement vector, resolving
 * collisions against a feature list (Quake-style iterative slide move).
 *
 * Starting from old_position with displacement old_velocity, each iteration tests the remaining move
 * against the collision features; on a hit it advances to the contact point, projects the remaining
 * displacement onto the struck plane, and accumulates the plane. A second concurrent hit slides along the
 * crease (line where the two planes meet); a third collapses to the corner (point where three planes meet),
 * stopping the move. Iteration continues until the residual move is negligible or maximum_collision_count
 * planes are recorded. The output velocity is old_velocity projected onto the surviving plane / crease
 * (zeroed at a corner). When two or more planes remain, a synthetic "floor" collision_plane is appended
 * combining the world-up vector with the steepest (most downward) struck plane, for the caller's pill fixup.
 *
 * Returns the number of collision planes written to `collisions`.
 *
 * DEVIATIONS from the decompiler:
 *   - The plane index stack was split across non-contiguous stack slots (v91/v92 vs v93..v95, coalesced by
 *     the compiler); reconstructed as two arrays active_stack[] (live) and new_stack[] (built this
 *     iteration, memcpy'd down at the tail).
 *   - v100 is initialised to old_velocity (dead) then reused for the three-plane corner point; modelled as
 *     `corner_point` (the initial copy is never read).
 *   - The decompiler recomputes the plane·velocity dot with interchanged (but numerically identical) normal
 *     components; folded into one `velocity_dot`. */

#include <stdint.h>
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include <math.h>
#include <string.h>
#include "headers/blam_data_globals.h"


extern uint8_t collision_features_test_vector(const collision_feature_list *features, const real_point3d *point, const real_vector3d *vector, collision_plane *collision);
extern uint8_t line_from_planes3d(const real_plane3d *plane0, const real_plane3d *plane1, real_point3d *point, real_vector3d *vector);
extern uint8_t point_from_planes3d(const real_plane3d *plane0, const real_plane3d *plane1, const real_plane3d *plane2, real_point3d *point);
extern float normalize3d(real_vector3d *v);

int16_t collision_move_point(
    const real_point3d *old_position,
    const real_vector3d *old_velocity,
    const collision_feature_list *features,
    real_point3d *new_position,
    real_vector3d *new_velocity,
    int16_t maximum_collision_count,
    collision_plane *collisions)
{
    const float eps = 0.000099999997f;

    float velocity_x = old_velocity->n[0];
    float velocity_y = old_velocity->n[1];
    float velocity_z = old_velocity->n[2];

    real_point3d corner_point;  /* decompiler seeds this with old_velocity (dead); reused for the corner */
    corner_point.n[0] = velocity_x;
    corner_point.n[1] = velocity_y;
    corner_point.n[2] = velocity_z;

    real_vector3d move;         /* remaining displacement to travel */
    move.n[0] = velocity_x;
    move.n[1] = velocity_y;
    move.n[2] = velocity_z;

    real_point3d position;
    position.n[0] = old_position->n[0];
    position.n[1] = old_position->n[1];
    position.n[2] = old_position->n[2];

    int collision_count = 0;
    int active_plane_count = 0;

    real_point3d crease_point;
    real_vector3d crease_line;
    float plane_x = 0, plane_y = 0, plane_z = 0, plane_d = 0;
    float last_plane_x = 0, last_plane_y = 0, last_plane_z = 0;  /* saved plane normal (single-plane slide) */
    float crease_x = 0, crease_y = 0, crease_z = 0;              /* saved crease direction (two-plane slide) */
    int16_t active_stack[3] = {0, 0, 0};
    int16_t new_stack[3];

    while ( fabsf(move.n[0]) >= eps || fabsf(move.n[1]) >= eps || fabsf(move.n[2]) >= eps )
    {
        int hit_index = (int16_t)collision_count;
        collision_plane *hit = &collisions[hit_index];
        uint8_t collided = collision_features_test_vector(features, &position, &move, hit);
        float contact_y = hit->point.n[1];

        if ( !collided )
        {
            /* no collision: advance to the end of the move and finish */
            crease_x = crease_line.n[0];
            crease_y = crease_line.n[1];
            crease_z = crease_line.n[2];
            position.n[0] = hit->point.n[0];
            position.n[1] = contact_y;
            position.n[2] = hit->point.n[2];
            break;
        }

        float contact_x = hit->point.n[0];
        float contact_z = hit->point.n[2];
        collision_count = (int16_t)(collision_count + 1);

        float remaining = 1.0f - hit->t;
        new_stack[0] = (int16_t)hit_index;

        velocity_y = velocity_y * remaining;
        velocity_z = velocity_z * remaining;
        velocity_x = velocity_x * remaining;

        int next_count = (int16_t)(hit_index + 1);
        int previous_active_count = active_plane_count;
        int new_active = 1;
        const real_plane3d *p_plane = &hit->plane;
        plane_x = hit->plane.n.n[0];
        plane_y = hit->plane.n.n[1];
        plane_z = hit->plane.n.n[2];
        plane_d = hit->plane.d;
        last_plane_x = plane_x;
        last_plane_y = plane_y;
        last_plane_z = plane_z;

        /* project the (scaled) velocity onto the struck plane; push the position out of penetration */
        float pushout = -((contact_x * plane_x + (contact_y * plane_y + contact_z * plane_z)) - plane_d);
        float velocity_dot = plane_x * velocity_x + (plane_z * velocity_z + plane_y * velocity_y);
        move.n[0] = plane_x * -velocity_dot + velocity_x;
        move.n[1] = plane_y * -velocity_dot + velocity_y;
        move.n[2] = plane_z * -velocity_dot + velocity_z;
        position.n[0] = pushout * plane_x + contact_x;
        position.n[1] = plane_y * pushout + contact_y;
        position.n[2] = plane_z * pushout + contact_z;

        if ( previous_active_count <= 0 )
            goto slide_done;

        /* already sliding on a plane: does the new move still drive into the previous plane? */
        collision_plane *prev = &collisions[active_stack[0]];
        const real_plane3d *prev_plane = &prev->plane;
        float into_prev = prev->plane.n.n[0] * move.n[0]
                        + (prev->plane.n.n[2] * move.n[2] + prev->plane.n.n[1] * move.n[1]);
        if ( into_prev < -eps )
        {
            /* slide along the crease where the current and previous planes meet */
            if ( line_from_planes3d(p_plane, &prev->plane, &crease_point, &crease_line) )
            {
                new_active = 2;
                crease_x = crease_line.n[0];
                crease_y = crease_line.n[1];
                new_stack[1] = active_stack[0];

                float crease_len2 = crease_line.n[2] * crease_line.n[2]
                                  + (crease_line.n[1] * crease_line.n[1] + crease_line.n[0] * crease_line.n[0]);
                float velocity_along = (crease_line.n[0] * velocity_x
                                      + (crease_line.n[1] * velocity_y + velocity_z * crease_line.n[2])) / crease_len2;
                move.n[0] = crease_line.n[0] * velocity_along;
                move.n[1] = crease_line.n[1] * velocity_along;
                move.n[2] = velocity_along * crease_line.n[2];

                /* project the contact point onto the crease line */
                float proj = ((contact_x - crease_point.n[0]) * crease_line.n[0]
                            + ((contact_y - crease_point.n[1]) * crease_line.n[1]
                             + (contact_z - crease_point.n[2]) * crease_line.n[2])) / crease_len2;
                position.n[0] = proj * crease_line.n[0] + crease_point.n[0];
                position.n[1] = crease_line.n[1] * proj + crease_point.n[1];
                position.n[2] = proj * crease_line.n[2] + crease_point.n[2];
                crease_z = crease_line.n[2];

                if ( previous_active_count <= 1 )
                    goto iteration_end;

                /* three planes: collapse to the corner if the crease move still drives into the third */
                collision_plane *third = &collisions[active_stack[1]];
                float into_third = third->plane.n.n[0] * move.n[0]
                                 + (third->plane.n.n[2] * move.n[2] + third->plane.n.n[1] * move.n[1]);
                if ( into_third >= -eps )
                    goto iteration_end;

                uint8_t corner_ok = point_from_planes3d(p_plane, prev_plane, &third->plane, &corner_point);
                crease_x = crease_line.n[0];
                crease_y = crease_line.n[1];
                crease_z = crease_line.n[2];
                if ( !corner_ok )
                    goto restore_move;
                move.n[0] = 0.0f;
                move.n[1] = 0.0f;
                move.n[2] = 0.0f;
                new_active = 3;
                new_stack[2] = active_stack[1];
                position = corner_point;
                goto iteration_end;
            }
            /* degenerate crease: fall through with the single-plane slide unchanged */
        }

        if ( previous_active_count <= 1 )
            goto slide_done;

        /* two planes but the previous does not block: try the crease with the second plane */
        collision_plane *other = &collisions[active_stack[1]];
        float into_other = other->plane.n.n[0] * move.n[0]
                         + (other->plane.n.n[2] * move.n[2] + other->plane.n.n[1] * move.n[1]);
        if ( into_other >= -eps )
            goto slide_done;

        if ( !line_from_planes3d(p_plane, &other->plane, &crease_point, &crease_line) )
        {
            crease_z = crease_line.n[2];
            goto restore_move;
        }
        new_active = 2;
        new_stack[1] = active_stack[1];
        {
            float crease_len2 = crease_line.n[2] * crease_line.n[2]
                              + (crease_line.n[1] * crease_line.n[1] + crease_line.n[0] * crease_line.n[0]);
            float velocity_along = (crease_line.n[0] * velocity_x
                                  + (crease_line.n[1] * velocity_y + velocity_z * crease_line.n[2])) / crease_len2;
            move.n[0] = velocity_along * crease_line.n[0];
            move.n[1] = crease_line.n[1] * velocity_along;
            move.n[2] = velocity_along * crease_line.n[2];

            float proj = ((contact_y - crease_point.n[1]) * crease_line.n[1]
                        + ((contact_z - crease_point.n[2]) * crease_line.n[2]
                         + (contact_x - crease_point.n[0]) * crease_line.n[0])) / crease_len2;
            position.n[0] = proj * crease_line.n[0] + crease_point.n[0];
            position.n[1] = crease_line.n[1] * proj + crease_point.n[1];
            position.n[2] = proj * crease_line.n[2] + crease_point.n[2];
            crease_z = crease_line.n[2];
        }
        goto iteration_end;

slide_done:
        crease_x = crease_line.n[0];
        crease_y = crease_line.n[1];
        crease_z = crease_line.n[2];
        goto iteration_end;

restore_move:
        /* move already equals the retained slide vector; no restore needed */

iteration_end:
        active_plane_count = new_active;
        memcpy(active_stack, new_stack, 2 * new_active);
        if ( next_count >= maximum_collision_count )
            break;
    }

    new_position->n[0] = position.n[0];
    new_position->n[1] = position.n[1];
    new_position->n[2] = position.n[2];

    if ( active_plane_count == 1 )
    {
        /* project the original velocity onto the surviving plane */
        float d = -(old_velocity->n[0] * last_plane_x + (last_plane_y * old_velocity->n[1] + last_plane_z * old_velocity->n[2]));
        new_velocity->n[0] = d * last_plane_x + old_velocity->n[0];
        new_velocity->n[1] = last_plane_y * d + old_velocity->n[1];
        new_velocity->n[2] = last_plane_z * d + old_velocity->n[2];
    }
    else if ( active_plane_count == 2 )
    {
        /* project the original velocity onto the crease direction */
        float len2 = crease_z * crease_z + (crease_y * crease_y + crease_x * crease_x);
        float t = (old_velocity->n[0] * crease_x + (old_velocity->n[1] * crease_y + old_velocity->n[2] * crease_z)) / len2;
        new_velocity->n[0] = t * crease_x;
        new_velocity->n[1] = crease_y * t;
        new_velocity->n[2] = t * crease_z;
    }
    else if ( active_plane_count )
    {
        new_velocity->n[0] = 0.0f;
        new_velocity->n[1] = 0.0f;
        new_velocity->n[2] = 0.0f;
    }
    else
    {
        new_velocity->n[0] = old_velocity->n[0];
        new_velocity->n[1] = old_velocity->n[1];
        new_velocity->n[2] = old_velocity->n[2];
    }

    if ( active_plane_count <= 1 || (int16_t)collision_count >= maximum_collision_count )
        return collision_count;

    /* append a synthetic floor plane from the steepest struck plane and world up */
    float steepest_slope = 0.0f;
    collision_plane *floor = &collisions[(int16_t)collision_count];
    collision_count = (int16_t)(collision_count + 1);
    int steepest_stack_index = -1;
    collision_plane *last_active = &collisions[active_stack[active_plane_count - 1]];
    floor->t = last_active->t;
    floor->point.n[0] = last_active->point.n[0];
    floor->point.n[1] = last_active->point.n[1];
    floor->point.n[2] = last_active->point.n[2];
    floor->object_index = -1;
    floor->surface_index = -1;
    floor->flags = 0;
    floor->breakable_surface_index = 0;
    floor->material_index = -1;

    if ( active_plane_count > 0 )
    {
        int i = 0;
        do
        {
            collision_plane *cp = &collisions[active_stack[i]];
            if ( steepest_slope > cp->plane.n.n[2] )
            {
                steepest_slope = cp->plane.n.n[2];
                steepest_stack_index = i;
            }
            ++i;
        }
        while ( i < active_plane_count );
    }

    float plane_dist;
    if ( active_plane_count != 2 )
    {
        if ( steepest_stack_index == -1 )
        {
            floor->plane.n = *global_up3d;
            plane_dist = floor->plane.n.n[0] * corner_point.n[0]
                       + (floor->plane.n.n[1] * corner_point.n[1] + floor->plane.n.n[2] * corner_point.n[2]);
        }
        else
        {
            collision_plane *steepest = &collisions[active_stack[steepest_stack_index]];
            float scale = -steepest->plane.n.n[2];
            floor->plane.n.n[0] = steepest->plane.n.n[0] * scale + global_up3d->n[0];
            floor->plane.n.n[1] = steepest->plane.n.n[1] * scale + global_up3d->n[1];
            floor->plane.n.n[2] = steepest->plane.n.n[2] * scale + global_up3d->n[2];
            if ( normalize3d(&floor->plane.n) == 0.0f )
                return (int16_t)(collision_count - 1);
            plane_dist = floor->plane.n.n[0] * corner_point.n[0]
                       + (corner_point.n[1] * floor->plane.n.n[1] + corner_point.n[2] * floor->plane.n.n[2]);
        }
        floor->plane.d = plane_dist;
        return collision_count;
    }

    /* two planes: floor normal is world-up with the crease component removed, or up × steepest */
    if ( steepest_stack_index == -1 )
    {
        float t = -(crease_z / (crease_z * crease_z + (crease_y * crease_y + crease_x * crease_x)));
        floor->plane.n.n[0] = t * crease_x + global_up3d->n[0];
        floor->plane.n.n[1] = crease_y * t + global_up3d->n[1];
        floor->plane.n.n[2] = t * crease_z + global_up3d->n[2];
    }
    else
    {
        collision_plane *steepest = &collisions[active_stack[steepest_stack_index]];
        float bx = steepest->plane.n.n[0];
        float by = steepest->plane.n.n[1];
        float bz = steepest->plane.n.n[2];
        if ( steepest_stack_index )
        {
            floor->plane.n.n[0] = by * crease_z - bz * crease_y;
            floor->plane.n.n[1] = bz * crease_x - bx * crease_z;
            floor->plane.n.n[2] = bx * crease_y - by * crease_x;
        }
        else
        {
            floor->plane.n.n[1] = bx * crease_z - bz * crease_x;
            floor->plane.n.n[0] = bz * crease_y - by * crease_z;
            floor->plane.n.n[2] = by * crease_x - bx * crease_y;
        }
    }
    if ( normalize3d(&floor->plane.n) != 0.0f )
    {
        floor->plane.d = crease_point.n[1] * floor->plane.n.n[1]
                              + (floor->plane.n.n[0] * crease_point.n[0] + crease_point.n[2] * floor->plane.n.n[2]);
        return collision_count;
    }
    return (int16_t)(collision_count - 1);
}
