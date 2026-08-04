/* player_control_get_target_object_index @0x836DE708 — the local player's current autoaim/action target
 * object index, or -1 if it no longer resolves to a live object of any type. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int player_control_get_target_object_index(int16_t local_player_index)
{
    player_control *control = &player_control_globals->players[local_player_index];

    if (object_try_and_get_and_verify_type(control->target_object_index, object_mask_all))
        return control->target_object_index;

    return -1;
}
