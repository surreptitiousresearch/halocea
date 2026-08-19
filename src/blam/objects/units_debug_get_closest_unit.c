/* units_debug_get_closest_unit @0x836CA4E8 — debug helper: returns the object index of the unit nearest
 * to unit_index (skipping itself and any object flagged 0x4 in its header word at +0x106). When called
 * with unit_index == -1 every candidate scores distance 0, so it returns the first eligible unit. */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float __fsqrts(float x);

int units_debug_get_closest_unit(int unit_index)
{
    float best_distance = 3.4028235e38f;
    int   best_index = -1;

    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_biped, 0);
    for (object_datum *object = object_iterator_next(&iterator);
         object;
         object = object_iterator_next(&iterator))
    {
        if (iterator.index == unit_index || (object->object.damage_flags & (1u << _object_dead_bit)) != 0)   /* object+262 */
            continue;

        float distance;
        if (unit_index == -1)
        {
            distance = 0.0f;
        }
        else
        {
            real_point3d from, to;
            object_get_origin(unit_index, &from);
            object_get_origin(iterator.index, &to);
            distance = __fsqrts((to.n[1] - from.n[1]) * (to.n[1] - from.n[1])
                                + ((to.n[0] - from.n[0]) * (to.n[0] - from.n[0])
                                   + (to.n[2] - from.n[2]) * (to.n[2] - from.n[2])));
        }

        if (distance < best_distance)
        {
            best_index = iterator.index;
            best_distance = distance;
        }
    }

    return best_index;
}
