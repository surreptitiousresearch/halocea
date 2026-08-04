/* scripted_music_stop_all @0x837146D0 — stop every scripted looping sound whose tag is a music sound.
 * Walks game_looping_sound_data; for each scripted entry (object_index == -1) it inspects the looping-sound
 * tag's tracks and treats it as music if any track's loop sound has sound class _sound_class_music. Matching sounds are
 * faded out. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/sound_definition.h"
#include "headers/sound_class.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern int data_next_index(const data_array *data, int16_t index);
extern void scripted_looping_sound_stop_internal(int definition_index, uint8_t fixed_fadeout);

extern uint8_t looping_sound_definition_is_music(int definition_index);
void scripted_music_stop_all(void)
{
    /* game_looping_sound_data is really a data_array* (DB); blam_data_globals mistypes it — cast. */
    int index = data_next_index(game_looping_sound_data, -1);
    while ( index != -1 )
    {
        game_looping_sound_datum *sound =
            DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum, index);
        if ( sound->object_index == -1 )
        {
            int tag_index = sound->definition_index;
            /* DEVIATION: collapsed inlined copy of looping_sound_definition_is_music@0x83714638 (0 xrefs) into a direct call; donor's `return 1`/`return 0` folds to this call site's is_music flag, donor's definition_index param is the local tag_index. */
            uint8_t is_music = looping_sound_definition_is_music(tag_index);

            if ( is_music )
                scripted_looping_sound_stop_internal(tag_index, 1u);
        }
        index = data_next_index(game_looping_sound_data, index);
    }
}
