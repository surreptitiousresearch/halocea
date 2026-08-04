/* game_sound_restore @0x83713898 — re-link every active looping sound (game_looping_sound datum, per
 * game_looping_sound_new.c: flags dword[1], definition tag dword[3]) back to its sound-looping tag
 * definition's cached instance-index slot (definition dword+28, not previously modeled): if the
 * definition's flags (dword[0]) bit 1 is set, the sound is deleted instead (definition no longer wants a
 * cached instance); otherwise the definition's slot is stamped with this datum's index. Only sounds whose
 * flags bit 0x10 is set are touched. Finally clears the same cached-instance slot to -1 on every "snd!"
 * (sound_looping) tag not visited above, and returns the last tag_iterator_next result (-1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_iterator.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/sound_definition.h"
#include "headers/looping_sound_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

int game_sound_restore(void)
{
    for ( int index = data_next_index(game_looping_sound_data, -1); index != -1;
          index = data_next_index(game_looping_sound_data, index) )
    {
        game_looping_sound_datum *sound =
            DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, index);
        if ( (sound->flags & (1u << _game_looping_sound_scripted_bit)) != 0 )
        {
            looping_sound_definition *definition = TAG_GET(looping_sound_definition, sound->definition_index);
            if ( (definition->flags & (1u << _looping_sound_fake_impulse_sound_bit)) != 0 )
                datum_delete(game_looping_sound_data, index);
            else
                definition->runtime_scripting_sound_index = index;
        }
    }

    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x736E6421u);   /* "snd!" (sound) */
    int result;
    for ( result = tag_iterator_next(&iterator); result != -1; result = tag_iterator_next(&iterator) )
        /* prior "no member at byte 144" note checked looping_sound_definition — but 'snd!' is the
         * SOUND tag; DB sound_definition offset 144 = runtime_scripting_time. */
        TAG_GET(sound_definition, result)->runtime_scripting_time = -1;
    return result;
}
