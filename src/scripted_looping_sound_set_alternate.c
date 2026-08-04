/* scripted_looping_sound_set_alternate @0x83713D58 — enable or disable the "alternate" variant of a running
 * scripted looping sound (game_looping_sound_datum.flags bit 0x8). The datum is found via the sound tag's
 * active looping-sound index (tag data +0x1C). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/looping_sound_definition.h"
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/blam_data_globals.h"


void scripted_looping_sound_set_alternate(int definition_index, uint8_t alternate)
{
    if ( definition_index == -1 )
        return;
    int looping_index = TAG_GET(looping_sound_definition, definition_index)->runtime_scripting_sound_index;   /* +28 */
    if ( looping_index == -1 )
        return;

    /* game_looping_sound_data is really a data_array* (DB: data_array *game_looping_sound_data);
       blam_data_globals types it as game_looping_sound_data_t* — cast to reach data_array.data/size. */
    game_looping_sound_datum *datum =
        DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum, looping_index);
    datum->flags = alternate ? (datum->flags | (1u << _game_looping_sound_alternate_bit)) : (datum->flags & ~(1u << _game_looping_sound_alternate_bit));
}
