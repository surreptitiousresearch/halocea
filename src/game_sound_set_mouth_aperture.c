/* game_sound_set_mouth_aperture @ 0x83714268 — record a unit's current mouth-open amount (for
 * lip-sync) when the looping-sound system is active. Fetches and type-checks the unit object.
 *
 * Deviation: the decompiler only recovered the validity gate and the unit fetch; the actual store
 * of `mouth_aperture` into the unit (the aperture argument is otherwise unused) was not present in
 * the output, so it is not reproduced here. */

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

#include "headers/game_looping_sound_data.h"
#include "headers/object_type.h"

void game_sound_set_mouth_aperture(int object_index, float mouth_aperture)
{
    if ( game_looping_sound_data->valid )
        object_try_and_get_and_verify_type(object_index, object_mask_unit);
}
