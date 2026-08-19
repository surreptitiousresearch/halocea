/* collision_fix_pill_nudge_collision @0x83774EA8 — back the contact point off a collided surface. Starting from the
 * plane's contact point, repeatedly steps the fraction t back by 1/32 along the test vector until the point clears
 * collision (collision_test_point false) or t reaches 0. If it never clears, the point is reset to the test
 * origin. Mutates collision->t and collision->point in place. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_plane.h"

extern uint8_t collision_test_point(unsigned int flags, const real_point3d *point, int ignore_object_index);

void collision_fix_pill_nudge_collision(unsigned int flags, const real_point3d *point, const real_vector3d *vector,
                                        int ignore_object_index, collision_plane *collision)
{
    if ( collision->t > 0.0 )
    {
        real_point3d *contact = &collision->point;
        do
        {
            if ( !collision_test_point(flags, contact, ignore_object_index) )
                break;
            float t = (collision->t - 0.03125f);
            collision->t = collision->t - 0.03125f;
            contact->n[0] = (vector->n[0] * t) + point->n[0];
            contact->n[1] = (vector->n[1] * t) + point->n[1];
            contact->n[2] = (vector->n[2] * t) + point->n[2];
        }
        while ( collision->t > 0.0 );
    }
    if ( collision->t <= 0.0 )
    {
        collision->point.n[0] = point->n[0];
        collision->point.n[1] = point->n[1];
        collision->point.n[2] = point->n[2];
    }
}
