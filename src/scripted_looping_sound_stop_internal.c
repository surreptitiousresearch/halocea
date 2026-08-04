/* scripted_looping_sound_stop_internal @0x837145A4 — stop a scripted looping sound: on its live game
 * looping-sound datum clear the scripted bit, set the "unattached stop requested" bit, unlink the
 * datum, and optionally request a fixed fadeout. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/looping_sound_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


void scripted_looping_sound_stop_internal(int definition_index, uint8_t fixed_fadeout)
{
    if ( definition_index != -1 )
    {
        looping_sound_definition *definition = TAG_GET(looping_sound_definition, definition_index);
        if ( definition->runtime_scripting_sound_index != -1 )
        {
            /* game_looping_sound_data is really a data_array* (DB); blam_data_globals mistypes it — cast. */
            game_looping_sound_datum *sound =
                DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum,
                                   definition->runtime_scripting_sound_index);
            sound->flags &= ~(1u << _game_looping_sound_scripted_bit);
            sound->flags |= (1u << _game_looping_sound_unattached_stop_bit);
            definition->runtime_scripting_sound_index = -1;
            if ( fixed_fadeout )
                sound->flags |= (1u << _game_looping_sound_unattached_stop_fixed_fadeout_bit);
        }
    }
}
