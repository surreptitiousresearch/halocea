/* unattached_looping_sound_stop @0x83713E44 — request that an unattached looping sound stop, by setting
 * the stop flag (bit 1) in its datum. The mixer tears the sound down on the next update. */

#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/blam_data_globals.h"


void unattached_looping_sound_stop(int looping_sound_index)
{
    game_looping_sound_datum *looping_sound =
        /* DEVIATION: blam_data_globals.h types game_looping_sound_data as game_looping_sound_data_t*
         * (stub); DB canonical is `data_array *`. Cast so the data-array idiom applies. */
        DATA_ARRAY_ELEMENT((data_array *)game_looping_sound_data, game_looping_sound_datum, looping_sound_index);
    looping_sound->flags |= (1u << _game_looping_sound_unattached_stop_bit);
}
