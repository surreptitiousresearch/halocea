/* observer_collision_test_with_t @0x8370F4D0 — cast a collision ray from p0 toward p1 for the third-person
 * camera and, on a hit, report the fraction along the ray at which it struck. Returns 1 and writes *t on a hit,
 * 0 otherwise. When ignore_media is set the media/water-surface bit (0x40) is cleared from the test flags so the
 * ray passes through water. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"

extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

uint8_t observer_collision_test_with_t(const real_point3d *p0, const real_point3d *p1, float *t,
                                       uint8_t ignore_media)
{
    /* front-facing structure + object + scenery-object surfaces (0x40A1); the media/water bit is
     * added (-> 0x40E1) unless ignore_media is set, so a water surface can stop the ray */
    unsigned int flags = (1u << _collision_test_front_facing_surfaces_bit)
                       | (1u << _collision_test_structure_bit)
                       | (1u << _collision_test_objects_bit)
                       | (1u << _collision_test_objects_scenery_bit);
    if ( !ignore_media )
        flags |= (1u << _collision_test_media_bit);

    real_vector3d vector;
    vector.n[0] = p1->n[0] - p0->n[0];
    vector.n[1] = p1->n[1] - p0->n[1];
    vector.n[2] = p1->n[2] - p0->n[2];

    collision_result collision;
    if (!collision_test_vector(flags, p0, &vector, -1, &collision))
        return 0;

    *t = collision.t;
    return 1;
}
