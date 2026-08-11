/* actor_perception_forget_recent_damage @0x837D6BB0 — clear the "currently damaging me" flag and reset
 * the damage-recency timer on every prop the actor perceives. */

#include <stdint.h>
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"

extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

void actor_perception_forget_recent_damage(int actor_index)
{
    prop_iterator iterator;
    prop_datum *prop;

    prop_iterator_new(&iterator, actor_index);
    for ( prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        prop->currently_damaging_me = 0;
        prop->ticks_since_damage = -1;
    }
}
