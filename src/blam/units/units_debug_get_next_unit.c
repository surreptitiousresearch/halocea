/* units_debug_get_next_unit @0x836CA3B0 — debug helper: returns the next "free" unit after unit_index in
 * iteration order, wrapping around to the start if needed. A unit is eligible only if it has no owners
 * recorded at +500/+504 (both -1) and lacks flag 0x4 in its header word at +0x106. Returns -1 if there
 * are no eligible units. */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_type.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);

static int unit_ineligible(const unit_datum *object)
{
    /* actor/swarm-actor owner indices (unit+500/+504) and damage_flags bit 2 (object+262) */
    return object->unit.actor_index != -1 || object->unit.swarm_actor_index != -1
        || (object->object.damage_flags & (1u << _object_dead_bit)) != 0;
}

int units_debug_get_next_unit(int unit_index)
{
    int result = -1;
    object_iterator iterator;

    if (unit_index != -1)
    {
        object_iterator_new(&iterator, object_mask_unit, 0);
        if (object_iterator_next(&iterator))
        {
            /* advance the cursor to unit_index */
            while (iterator.index != unit_index && object_iterator_next(&iterator))
                ;
        }

        /* scan forward from just past unit_index for the next eligible unit */
        unit_datum *object = object_iterator_next(&iterator);
        if (object)
        {
            while (unit_ineligible(object))
            {
                object = object_iterator_next(&iterator);
                if (!object)
                    goto wrap;
            }
            result = iterator.index;
            if (iterator.index != -1)
                return result;
        }
    }

wrap:
    /* wrap: scan from the very first unit */
    object_iterator_new(&iterator, object_mask_unit, 0);
    for (unit_datum *object = object_iterator_next(&iterator); object;
         object = object_iterator_next(&iterator))
    {
        if (!unit_ineligible(object))
            return iterator.index;
    }
    return result;
}
