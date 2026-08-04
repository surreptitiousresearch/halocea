/* unit_try_drop_one_grenade @0x836CC710 — spawn and drop one grenade item from the unit's inventory. Creates
 * the grenade object from the game-globals grenade definition, disconnects it from the map so it can fall,
 * hands it to unit_drop_item, and decrements the unit's carried count for that grenade type. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_placement_data.h"
#include "headers/game_globals.h"
#include "headers/blam_data_globals.h"

extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_disconnect_from_map(int object_index);
extern void unit_drop_item(int unit_index, int item_index);

void unit_try_drop_one_grenade(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    object_placement_data placement;

    /* grenades.address element dword 29 = grenade object definition index (see game_globals.h) */
    object_placement_data_new(&placement, ((int *)global_game_globals->grenades.address)[29], unit_index);

    int grenade_object_index = object_new(&placement);
    if (grenade_object_index != -1)
    {
        object_disconnect_from_map(grenade_object_index);
        unit_drop_item(unit_index, grenade_object_index);
    }

    if (unit->unit.grenade_counts[1] > 0)
        --unit->unit.grenade_counts[1];
}
