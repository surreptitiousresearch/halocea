/* check_is_local_player_by_loop @ 0x83714820 — true if the object owning a looping sound is a player
 * controlled locally (has a valid local-player slot). Resolves the looping sound's owner object
 * (+16 of the game-looping-sound datum), reads its player index (object +192, dword idx48), looks up
 * the player datum, and checks the player's local-machine slot (word +2) is assigned. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);

uint8_t check_is_local_player_by_loop(uint16_t looping_sound_index)
{
    object_datum *object = object_try_and_get_and_verify_type(
                      DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, looping_sound_index)->object_index, object_mask_all);
    player_datum *player;

    if ( !object )
        return 0;

    player = datum_try_and_get(player_data, object->object.owner_player_index);  /* object+0xC0 */
    if ( !player )
        return 0;

    /* local_player_index is signed __int16; keep the unsigned cast for the 0xFFFF sentinel compare */
    if ( (uint16_t)player->local_player_index == 0xFFFF )
        return 0;
    return 1;
}
