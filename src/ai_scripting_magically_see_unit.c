/* ai_scripting_magically_see_unit @0x8376F9F0 — make every actor addressed by an AI index instantly aware
 * of a unit, as if they had perceived it: their encounter is force-activated, and a perception prop is
 * acquired for the unit and handled with the "fully seen" effect type. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/ai_unit_effect.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void encounter_force_activate(int encounter_index);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_handle_unit_effect(int actor_index, int prop_index, int16_t effect_type);

void ai_scripting_magically_see_unit(int ai_index, int unit_index)
{
    if ( ai_index == -1 || unit_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        int encounter_index = actor->meta.encounter_index;
        if ( encounter_index != -1 )
            encounter_force_activate(encounter_index);
        int prop_index = prop_get_base_by_unit_index(iterator.iterator.index, unit_index, 1u, 0);
        if ( prop_index != -1 )
            actor_handle_unit_effect(iterator.iterator.index, prop_index, _ai_unit_effect_magic_sight);
    }
}
