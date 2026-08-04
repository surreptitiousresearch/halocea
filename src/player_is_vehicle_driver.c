/* player_is_vehicle_driver @0x836A8B80 — true if the player is currently driving a vehicle: their unit
 * (player record dword +13) must be a unit object whose parent vehicle (dword +71) exists and whose
 * occupied seat (word +376) is that vehicle's driver seat. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t unit_seat_is_driver(int vehicle_index, int16_t seat_index);

/* attested uint8_t: all 6 sampled callers clrlwi r3,24 (byte-normalize); success path is a raw
 * unit_seat_is_driver r3 passthrough @0x836A8BB4 */
uint8_t player_is_vehicle_driver(int player_index)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( player )
    {
        unit_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
        if ( unit )
        {
            int vehicle_index = unit->object.parent_object_index;
            if ( vehicle_index != -1 )
                return unit_seat_is_driver(vehicle_index, unit->unit.parent_seat_index);
        }
    }
    return 0;
}
