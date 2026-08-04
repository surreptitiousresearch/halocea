/* update_potentially_audible_looping_sound_not @0x83713E58 — per-frame service of one active looping sound
 * that is NOT currently in the potentially-audible set (counterpart of
 * update_potentially_audible_looping_sound; same game_looping_sound_datum layout). Computes whether the sound should still be heard (fixed-scale flag word or the attached
 * object's function value) and, if it is audible — or not yet fully stopped and not serviced last frame —
 * refreshes it through the not-audible path (which winds it down / keeps its bookkeeping alive). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_sound_globals.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/game_looping_sound_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void sound_refresh_looping_not(int identifier, uint8_t alternate);

void update_potentially_audible_looping_sound_not(int sound_index)
{
    game_looping_sound_datum *sound =
        DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, sound_index);
    int last_frame = sound->last_audible_frame_index;
    char serviced_last_frame = (last_frame == -1 || last_frame == game_sound_globals->frame_index - 1);
    int flags = sound->flags;

    unsigned __int8 audible;
    float function_value;
    if ( (flags & (1u << _game_looping_sound_unattached_bit)) != 0 )
        audible = (~flags >> _game_looping_sound_unattached_stop_bit) & 1;  /* unattached: audible unless stop bit set */
    else
        audible = object_get_function_value(sound->object_index, sound->attachment.function_index,
                                            &function_value);

    if ( audible || (sound->state != _game_looping_sound_inactive && serviced_last_frame) )
        sound_refresh_looping_not(sound_index, (sound->flags & (1u << _game_looping_sound_alternate_bit)) != 0);
}
