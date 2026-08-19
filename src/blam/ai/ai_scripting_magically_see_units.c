/* ai_scripting_magically_see_units @0x8376FA90 — make every actor addressed by an AI index instantly aware
 * of each unit in an object list (force-activating their encounter and acquiring a perception prop with the
 * "fully seen" effect for every unit). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/ai_unit_effect.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void encounter_force_activate(int encounter_index);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_handle_unit_effect(int actor_index, int prop_index, int16_t effect_type);

void ai_scripting_magically_see_units(int ai_index, int object_list_index)
{
    int iterator_state[4];
    for ( int unit_index = object_list_get_first(object_list_index, iterator_state);
          unit_index != -1;
          unit_index = object_list_get_next(object_list_index, iterator_state) )
    {
        if ( ai_index == -1 )
            continue;

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
}
