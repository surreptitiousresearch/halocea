#include <stdint.h>
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"

extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

void actor_perception_retreat_successful(int actor_index)
{
    prop_iterator iterator;

    prop_iterator_new(&iterator, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        prop->unopposable_trigger_hysteresis = 0;
        prop->unopposable_trigger_threshold = 0;
        prop->unopposable_trigger_timer = 0;
    }
}
