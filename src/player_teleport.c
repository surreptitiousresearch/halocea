/* player_teleport @0x836AB188 — teleport a player's unit to a position. The unit (player record dword
 * +52) must be a valid object; if it currently occupies a seat (dword +71 set), eject it first, then
 * perform the actual teleport. */

#include <stdint.h>
#include "headers/object_type.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t player_teleport_internal(int player_index, int source_unit_index, const real_point3d *position);

uint8_t player_teleport(int player_index, int source_unit_index, const real_point3d *position)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    object_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
    if ( unit )
    {
        if ( unit->object.parent_object_index != -1 )   /* +284 */
            unit_exit_seat_end(unit_index, 0, 0, 1u);
        return player_teleport_internal(player_index, source_unit_index, position);
    }
    return 0;
}
