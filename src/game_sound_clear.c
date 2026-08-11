/* game_sound_clear @0x83713810 — clear the back-reference from every looping-sound definition to its
 * live game looping-sound datum, so the next map's sounds re-bind cleanly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);

void game_sound_clear(void)
{
    for ( int index = data_next_index(game_looping_sound_data, -1);
          index != -1;
          index = data_next_index(game_looping_sound_data, index) )
    {
        game_looping_sound_datum *loop = DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, index);
        looping_sound_definition *definition =
            TAG_GET(looping_sound_definition, loop->definition_index);
        if ( definition->runtime_scripting_sound_index == index )
            definition->runtime_scripting_sound_index = -1;
    }
}
