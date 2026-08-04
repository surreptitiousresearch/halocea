/* scripted_looping_sound_start @0x83714EC8 — start a scripted looping sound, optionally attached to a source
 * object. Any previous instance is stopped first; if the looping-sound definition is flagged
 * "stops music" (looping_sound_definition.flags bit _looping_sound_stops_music_bit) all other scripted
 * music is stopped. The resulting looping-sound datum index is recorded on the sound tag
 * (looping_sound_definition.runtime_scripting_sound_index) and the datum is flagged scripted
 * (game_looping_sound_datum.flags bit _game_looping_sound_scripted_bit). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_definition_flags.h"
#include "headers/blam_data_globals.h"

extern void scripted_looping_sound_stop_internal(int definition_index, uint8_t fixed_fadeout);
extern void scripted_music_stop_all(void);
extern int unattached_looping_sound_start(int definition_index, int source_object_index, float scale);

void scripted_looping_sound_start(int definition_index, int source_object_index, float scale)
{
    if ( definition_index == -1 )
        return;

    looping_sound_definition *definition = TAG_GET(looping_sound_definition, definition_index);
    scripted_looping_sound_stop_internal(definition_index, 0);
    if ( (definition->flags & (1u << _looping_sound_stops_music_bit)) != 0 )
        scripted_music_stop_all();

    int looping_index = unattached_looping_sound_start(definition_index, source_object_index, scale);
    definition->runtime_scripting_sound_index = looping_index;
    if ( looping_index != -1 )
    {
        /* game_looping_sound_data is really a data_array* (DB); blam_data_globals mistypes it — cast. */
        DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum, looping_index)->flags |=
            (1u << _game_looping_sound_scripted_bit);
    }
}
