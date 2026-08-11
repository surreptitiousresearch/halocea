/* actor_expected_acknowledgement @0x837D6838 — decide whether the actor would already expect to acknowledge
 * a given prop, by scanning its other props for one that refers to the same unit/actor (or a nearby active
 * enemy) within ~2.5 world units, ~1.5 vertically, and roughly in front (facing dot > 0.5). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

int actor_expected_acknowledgement(int actor_index, int prop_index)
{
    prop_datum *target = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    uint8_t expected = 0;
    prop_iterator iterator;

    prop_iterator_new(&iterator, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        if ( iterator.index == prop_index )
            continue;
        if ( prop->unit_index == target->unit_index
          || prop->actor_index == target->actor_index
          || (target->enemy && prop->enemy
              && ((prop->state >= _prop_state_uninspected_orphan && prop->state <= _prop_state_inspected_orphan) || (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged))) )
        {
            float dy = target->body_position.n[1] - prop->body_position.n[1];
            float dx = target->body_position.n[0] - prop->body_position.n[0];
            if ( (dx * dx) + (dy * dy) < 6.25f
              && __fabs(prop->body_position.n[2] - target->body_position.n[2]) < 1.5f
              && (target->actor_to_prop.n[0] * prop->actor_to_prop.n[0])
                   + (target->actor_to_prop.n[2] * prop->actor_to_prop.n[2])
                   + (target->actor_to_prop.n[1] * prop->actor_to_prop.n[1]) > 0.5f )
            {
                expected = 1;
            }
        }
    }
    return expected;
}
