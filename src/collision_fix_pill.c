/* collision_fix_pill @0x83774F88 — nudge a pill (capsule) out of nearby geometry. Gathers collision features in a
 * sphere around the pill, then: if the original position is already clear, keeps it; otherwise probes 17 fixed
 * offset directions for a clear spot. A clear spot with walkable ground beneath (surface up-component > cos40°) is
 * nudged off the contacted plane and returned. Failing that, the first merely-clear spot found is used as a
 * fallback. Returns TRUE if a valid position was produced.
 *
 * Deviation: DB prototype is authoritative (7 params). Hex-Rays mis-modelled the three float params (distance/
 * height/width GPR-skip) and exploded the large stack frame into dozens of phantom args. Verified via prologue:
 * flags=r3, old_position=r4, distance=f1, height=f2, width=f3, ignore_object_index=r8, new_position=r9. The first
 * decompiler "call" to RtlCheckStack12 is the compiler stack probe, not a value source. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_plane.h"
#include "headers/collision_feature_list.h"
#include "headers/blam_data_globals.h"


extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern uint8_t collision_features_test_point(const collision_feature_list *features, const real_point3d *point, collision_plane *collision);
extern uint8_t collision_test_point(unsigned int flags, const real_point3d *point, int ignore_object_index);
extern uint8_t collision_features_test_vector(const collision_feature_list *features, const real_point3d *point, const real_vector3d *vector, collision_plane *collision);
extern void collision_fix_pill_nudge_collision(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_plane *collision);

uint8_t collision_fix_pill(unsigned int flags, const real_point3d *old_position, float distance,
                                   float height, float width, int ignore_object_index, real_point3d *new_position)
{
    collision_feature_list features;
    collision_plane first_plane;
    collision_plane plane;
    real_point3d center;
    real_point3d candidate;
    real_point3d fallback;
    real_vector3d ground_vector;
    int succeeded = 0;

    center.n[0] = old_position->n[0];
    center.n[1] = old_position->n[1];
    center.n[2] = (height * 0.5f) + old_position->n[2];
    float radius = (((height * 0.5f) + distance) + width);
    collision_get_features_in_sphere(flags, &center, radius, height, width, ignore_object_index, &features);

    if ( !collision_features_test_point(&features, old_position, &first_plane)
      && !collision_test_point(flags, old_position, ignore_object_index) )
    {
        new_position->n[0] = old_position->n[0];
        new_position->n[1] = old_position->n[1];
        new_position->n[2] = old_position->n[2];
        return 1;
    }

    char have_fallback = 0;
    int16_t i = 0;
    while ( 1 )
    {
        int16_t current = i;
        candidate.n[0] = (offsets[i].n[0] * distance) + old_position->n[0];
        candidate.n[1] = (offsets[i].n[1] * distance) + old_position->n[1];
        candidate.n[2] = (offsets[i].n[2] * distance) + old_position->n[2];

        if ( !collision_features_test_point(&features, &candidate, &plane)
          && !collision_test_point(flags, &candidate, ignore_object_index) )
        {
            /* candidate is clear — require walkable ground beneath it */
            ground_vector.n[0] = global_down3d->n[0] * distance;
            ground_vector.n[1] = global_down3d->n[1] * distance;
            ground_vector.n[2] = global_down3d->n[2] * distance;
            if ( collision_features_test_vector(&features, &candidate, &ground_vector, &plane)
              && plane.plane.normal.n[2] > 0.76604444f )   /* ground normal up-component == walkable slope */
            {
                collision_fix_pill_nudge_collision(flags, &candidate, &ground_vector, ignore_object_index, &plane);
                new_position->n[0] = plane.point.n[0];
                new_position->n[1] = plane.point.n[1];
                new_position->n[2] = plane.point.n[2];
                succeeded = 1;
                break;
            }
            if ( !have_fallback )
            {
                have_fallback = 1;
                fallback = candidate;
            }
        }

        i = current + 1;
        if ( (uint16_t)(current + 1) >= 0x11u )
            break;
    }

    if ( succeeded || !have_fallback )
        return succeeded;

    /* no good ground anywhere; settle on the first clear fallback spot */
    ground_vector.n[0] = old_position->n[0] - fallback.n[0];
    ground_vector.n[1] = old_position->n[1] - fallback.n[1];
    ground_vector.n[2] = old_position->n[2] - fallback.n[2];
    collision_features_test_vector(&features, &fallback, &ground_vector, &plane);
    collision_fix_pill_nudge_collision(flags, &fallback, &ground_vector, ignore_object_index, &plane);
    new_position->n[0] = plane.point.n[0];
    new_position->n[1] = plane.point.n[1];
    new_position->n[2] = plane.point.n[2];
    return 1;
}
