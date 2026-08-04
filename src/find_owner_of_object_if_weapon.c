/* find_owner_of_object_if_weapon @0x83739BF8 — scan all players for one whose unit's currently-readied
 * weapon (unit inventory slot from the unit's current-weapon word +754) is `object_index`. Returns 0 when
 * no player owns it; otherwise 1 when the owning player is local, or -1 when the owner's local-player word
 * (player datum +2) is NONE (a remote player). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags); /* DB proto: returns void* */
extern int unit_inventory_get_weapon(int unit_index, int16_t index);

int16_t find_owner_of_object_if_weapon(int object_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        int unit_index = player->unit_index;
        if ( !object_try_and_get_and_verify_type(unit_index, object_mask_unit) )
            continue;

        unit_datum *unit =
            (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        if ( object_index == unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index) )
            return (unsigned __int16)player->local_player_index == 0xFFFF ? -1 : 1;
    }
    return 0;
}
