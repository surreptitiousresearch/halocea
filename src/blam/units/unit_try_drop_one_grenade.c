/* unit_try_drop_one_grenade @0x836CC710 — spawn and drop one grenade item from the unit's inventory. Creates
 * the grenade object from the game-globals grenade definition, disconnects it from the map so it can fall,
 * hands it to unit_drop_item, and decrements the unit's carried count for that grenade type. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_placement_data.h"
#include "headers/game_globals.h"
#include "headers/game_globals_grenade.h"
#include "headers/unit_grenade_type.h"
#include "headers/blam_data_globals.h"

extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_disconnect_from_map(int object_index);
extern void unit_drop_item(int unit_index, int item_index);

void unit_try_drop_one_grenade(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    object_placement_data placement;

    /* DEVIATION: the decompiler's `((int *)grenades.address)[29]` (byte 0x74) is
     * grenades[_unit_grenade_covenant_plasma].item.index — element 1 of the 68-byte game_globals_grenade
     * table (68) plus tag_reference.index inside `item` (0x24 + 0x0C). Disasm 0x836CC74C/0x836CC754:
     * lwz r6, 0x12C(game_globals) then lwz r4, 0x74(r6). The grenade type is hard-coded here, matching
     * the equally hard-coded grenade_counts[1] decrement below. */
    object_placement_data_new(&placement,
            ((const game_globals_grenade *)global_game_globals->grenades.address)[_unit_grenade_covenant_plasma].item.index,
            unit_index);

    int grenade_object_index = object_new(&placement);
    if (grenade_object_index != -1)
    {
        object_disconnect_from_map(grenade_object_index);
        unit_drop_item(unit_index, grenade_object_index);
    }

    if (unit->unit.grenade_counts[_unit_grenade_covenant_plasma] > 0)
        --unit->unit.grenade_counts[_unit_grenade_covenant_plasma];
}
