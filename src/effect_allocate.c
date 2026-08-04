/* effect_allocate @0x836E26C0 — reserve an effect datum for a definition. Skips allocation in the editor
 * or for a missing definition. Non-deterministic effects are dropped when the definition is flagged
 * deterministic-only (flag 0x4) and the caller can't guarantee determinism; when the pool is full, a
 * deterministic effect may evict the first non-deterministic instance. Returns the datum index or -1.
 *
 * effect datum (252 bytes): definition dword[1], owner object dword[16] (@64), local player index
 * word[38] (@76) = -1, flags word[1] (@2) = 0. Effect definition (v7): flags dword[0] (0x4 = deterministic),
 * event count dword[13] (@52). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern uint8_t game_in_editor(void);
extern int datum_new(data_array *data);
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);
extern void effect_set_event(int effect_index, int16_t event_index);

int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic)
{
    int index = -1;
    effect_definition *definition;
    effect_datum *effect;

    if ( game_in_editor() || definition_index == -1 )
        return -1;

    definition = TAG_GET(effect_definition, definition_index);
    if ( (!can_be_deterministic && (definition->flags & (1u << _effect_must_be_deterministic_bit)) != 0) || definition->events.count <= 0 )
        return index;

    index = datum_new(effect_data);
    if ( index == -1 )
    {
        if ( (definition->flags & (1u << _effect_must_be_deterministic_bit)) == 0 )
            return index;

        /* pool full: evict the first non-deterministic effect to make room */
        index = data_next_index(effect_data, -1);
        if ( index == -1 )
            return index;
        while ( (TAG_GET(effect_definition, DATA_ARRAY_ELEMENT(effect_data, effect_datum, index)->definition_index)->flags & (1u << _effect_must_be_deterministic_bit)) != 0 )
        {
            index = data_next_index(effect_data, index);
            if ( index == -1 )
                return index;
        }
        datum_delete(effect_data, index);
        index = datum_new(effect_data);
        if ( index == -1 )
            return index;
    }

    effect = DATA_ARRAY_ELEMENT(effect_data, effect_datum, index);
    effect->definition_index = definition_index;
    effect->owner_object_index = owner_object_index;
    effect->local_player_index = -1;
    effect->flags = 0;
    effect_set_event(index, 0);
    return index;
}
