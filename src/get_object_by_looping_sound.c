/* get_object_by_looping_sound @ 0x837147B8 — return the object index that owns a looping sound, or
 * -1 if there is none or the owner object no longer exists. The owner handle is stored at +16 (dword
 * idx 4) of the 52-byte game-looping-sound datum; it is validated through the object table. */

#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int get_object_by_looping_sound(int sound_index)
{
    int *object;

    if ( sound_index == -1 )
        return -1;

    object = object_try_and_get_and_verify_type(
                 DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, sound_index)->object_index,
                 object_mask_all);
    if ( !object )
        return -1;
    return *object;
}
