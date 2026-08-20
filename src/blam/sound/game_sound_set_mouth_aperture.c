/* game_sound_set_mouth_aperture @ 0x83714268 — record a unit's current mouth-open amount (for
 * lip-sync) when the looping-sound system is active. Fetches and type-checks the unit object.
 *
 * FAITHFUL QUIRK: there is no store. The whole function is `lbz r10,0x24(r11); cmplwi cr6,r10,0;
 * beqlr cr6; li r4,3; b object_try_and_get_and_verify_type` (0x83714270-0x83714280) — the
 * `mouth_aperture` float arrives in f1 and is never read, and nothing is written to the unit. The
 * reconstruction is complete, not truncated: the shipped binary discards the aperture. */

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

#include "headers/game_looping_sound_data.h"
#include "headers/object_type.h"

void game_sound_set_mouth_aperture(int object_index, float mouth_aperture)
{
    if ( game_looping_sound_data->valid )
        object_try_and_get_and_verify_type(object_index, object_mask_unit);
}
