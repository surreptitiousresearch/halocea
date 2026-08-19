/* play_weapon_hud_sounds @0x836AF808 — resolve the local player's unit for HUD weapon-sound
 * playback. When the HUD is valid (hud_index != -1) and the local player maps to a real player,
 * the player's unit index is fetched and verified to be a biped/vehicle
 * (object_try_and_get_and_verify_type mask 3). The weapon_state argument is passed by callers but
 * is not referenced in this build (the sound-emission body is inlined/compiled out). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/weapon_interface_state.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void play_weapon_hud_sounds(int16_t local_player_index, int hud_index, weapon_interface_state *weapon_state)
{
    (void)weapon_state;

    if (hud_index != -1)
    {
        int player_index = local_player_get_player_index(local_player_index);
        int object_index = player_index;
        if (player_index != -1)
            object_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                                              local_player_get_player_index(local_player_index))->unit_index;
        object_try_and_get_and_verify_type(object_index, object_mask_unit);
    }
}
