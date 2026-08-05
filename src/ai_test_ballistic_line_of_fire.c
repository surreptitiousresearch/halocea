/* ai_test_ballistic_line_of_fire @0x836E8650 — test whether an actor has a clear line of fire along a
 * ballistic arc (e.g. a thrown grenade or lobbed projectile). The arc starts at `origin` with velocity
 * `arc_initial_velocity` under constant `arc_acceleration` (gravity on z) and is marched in segments of up
 * to 6 seconds out to `arc_time`. Each segment is raycast against world geometry (collision_test_vector,
 * ignoring `ignore_object_index`, and ignoring vehicles when requested) and against the actor's friendly
 * line-of-fire pills. Returns 1 if the whole arc is clear, 0 if any segment hits world geometry or passes
 * through a friendly pill.
 *
 * DEVIATION: this is the FPR-shadow trap — the DB prototype (7 args) is ground truth, but the two float
 * args (arc_time in f1, arc_acceleration in f2) each reserve a skipped GPR slot, so the decompiler invented
 * phantom args and mislabeled r6/r8/r9. Disasm confirms: r4=origin, r6=arc_initial_velocity (the
 * decompiler's float* "ignore_object_index"), r8=ignore_object_index (its "a8"), r9=ignore_vehicles (its
 * "a9"); collision flags = ignore_vehicles ? 49331 : 49843. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"
#include "headers/line_of_fire_pill.h"

extern int16_t ai_find_line_of_fire_friend_pills(int actor_index, int16_t max_pill_count, line_of_fire_pill *pills);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t vector_intersects_pill3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *base, const real_vector3d *height, float width);

uint8_t ai_test_ballistic_line_of_fire(int actor_index, const real_point3d *origin, float arc_time,
                                       const real_vector3d *arc_initial_velocity, float arc_acceleration,
                                       int ignore_object_index, uint8_t ignore_vehicles)
{
    line_of_fire_pill friend_pills[34];
    int16_t pill_count = ai_find_line_of_fire_friend_pills(actor_index, 32, friend_pills);

    /* 49843 = 0xC2B3; with vehicles cleared -> 49331 = 0xC0B3 (matches ignore_vehicles path). */
    unsigned int collision_flags =
          (1u << _collision_test_front_facing_surfaces_bit)
        | (1u << _collision_test_back_facing_surfaces_bit)
        | (1u << _collision_test_ignore_breakable_surfaces_bit)
        | (1u << _collision_test_structure_bit)
        | (1u << _collision_test_objects_bit)
        | (1u << _collision_test_objects_vehicles_bit)
        | (1u << _collision_test_objects_scenery_bit)
        | (1u << _collision_test_objects_machines_bit);
    if ( ignore_vehicles )
        collision_flags &= ~(1u << _collision_test_objects_vehicles_bit);

    real_point3d segment_start = *origin;
    float vx = arc_initial_velocity->n[0];
    float vy = arc_initial_velocity->n[1];
    float vz = arc_initial_velocity->n[2];

    float segment_t0 = 0.0f;
    float segment_t1 = (arc_time >= 6.0f) ? 6.0f : arc_time;

    while (1)
    {
        float dt = segment_t1 - segment_t0;

        real_point3d segment_end;
        segment_end.n[0] = vx * dt + segment_start.n[0];
        segment_end.n[1] = vy * dt + segment_start.n[1];
        segment_end.n[2] = 0.5f * dt * dt * arc_acceleration + (dt * vz + segment_start.n[2]);

        real_vector3d segment_delta;
        segment_delta.n[0] = segment_end.n[0] - segment_start.n[0];
        segment_delta.n[1] = segment_end.n[1] - segment_start.n[1];
        segment_delta.n[2] = segment_end.n[2] - segment_start.n[2];

        collision_result result;
        uint8_t hit = collision_test_vector(collision_flags, &segment_start, &segment_delta,
                                                    ignore_object_index, &result);
        uint8_t clear = (hit == 0);
        if (hit)
            return clear; /* blocked by world geometry */

        /* segment clear of world geometry; reject if it passes through a friendly pill */
        if (pill_count > 0)
        {
            for (int i = 0; i < pill_count; i++)
            {
                if (vector_intersects_pill3d(&segment_start, &segment_delta, &friend_pills[i].base,
                                             &friend_pills[i].directed_height, friend_pills[i].width))
                {
                    clear = 0;
                    break;
                }
            }
        }

        if (!clear)
            return clear; /* blocked by friendly pill */

        /* advance to the next arc segment, integrating gravity into the z velocity */
        segment_t0 = segment_t1;
        segment_t1 += 6.0f;
        vz = dt * arc_acceleration + vz;
        segment_start = segment_end;
        if (segment_t1 > arc_time)
            segment_t1 = arc_time;

        if (segment_t0 >= arc_time)
            return clear; /* reached the end of the arc with a clear line */
    }
}
