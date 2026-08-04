/* ai_scripting_free_units @0x83771B48 — detach the controlling actor of every unit in the object list from
 * its encounter, converting each to a free-roaming "encounterless" actor. Units without an actor, or whose
 * actor is already encounterless, are skipped. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void actor_flush_position_indices(uint16_t actor_index);
extern void encounter_detach_actor(int actor_index, uint8_t died);
extern void encounterless_attach_actor(int actor_index);
extern void encounters_update_dirty_status(void);

void ai_scripting_free_units(int object_list_index)
{
    int iterator[12];
    __int16 freed_count = 0;
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        unit_datum *unit = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( !unit )
            continue;
        int actor_index = unit->unit.actor_index;
        /* only detach actors that belong to an encounter. */
        if ( actor_index != -1 && DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.encounter_index != -1 )
        {
            actor_flush_position_indices(actor_index);
            encounter_detach_actor(actor_index, 0);
            encounterless_attach_actor(actor_index);
            ++freed_count;
        }
    }
    if ( freed_count > 0 )
        encounters_update_dirty_status();
}
