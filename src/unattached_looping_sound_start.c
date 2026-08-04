/* unattached_looping_sound_start @0x83713DB8 — start a looping sound not attached to any object (the
 * scenario background loop). Allocates a looping-sound datum, and on success stores the gain scale and
 * marks it unattached (bit 0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/blam_data_globals.h"

extern unsigned __int8 empty_string;

/* def (src/game_looping_sound_new.c): arg3 is `const char *marker_name`, arg4 is `__int16 function_index`.
 * disasm (0x83713DE8) confirms the caller loads the ADDRESS of empty_string into r5 as marker_name. */
extern int game_looping_sound_new(int object_index, int definition_index, const char *marker_name, int16_t function_index);

int unattached_looping_sound_start(int definition_index, int source_object_index, float scale)
{
    int index = game_looping_sound_new(source_object_index, definition_index, (const char *)&empty_string, -1);
    if ( index != -1 )
    {
        /* DEVIATION: blam_data_globals.h types game_looping_sound_data as game_looping_sound_data_t*
         * (stub); DB canonical is `data_array *`. Cast so the data-array idiom applies. */
        game_looping_sound_datum *sound = DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum, index);
        sound->scale = scale;
        sound->flags |= (1u << _game_looping_sound_unattached_bit);
    }
    return index;
}
