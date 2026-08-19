/* unit_add_grenade_to_inventory @0x836CB030 — try to absorb a grenade equipment pickup into a unit's grenade
 * inventory. Looks up the grenade type from the equipment's definition, and if the unit is below the per-type
 * maximum (from game_globals->grenades), increments the count, flags the unit object, fires the pickup HUD
 * message for local players, deletes the equipment object, and returns 1. Returns 0 if already at the cap. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/equipment_definition.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_grenade.h"
#include "headers/global_tag_instances.h"
#include "headers/player_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);
extern void equipment_handle_pickup(int equipment_index);
extern void object_delete(int object_index);

uint8_t unit_add_grenade_to_inventory(int unit_index, int equipment_index)
{
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    object_datum *equipment_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, equipment_index)->datum;

    equipment_definition *definition = TAG_GET(equipment_definition, equipment_object->definition_index);
    int16_t grenade_type = definition->equipment.grenade_type;
    game_globals_grenade *grenade_globals =
        &((game_globals_grenade *)global_game_globals->grenades.address)[grenade_type];

    if (grenade_globals && unit_object->unit.grenade_counts[grenade_type] < grenade_globals->maximum_count)
    {
        ++unit_object->unit.grenade_counts[definition->equipment.grenade_type];
        unit_object->object.flags |= (1u << _object_force_baseline_update_bit);
        if (player_index_from_unit_index(unit_index) != -1
            && DATA_ARRAY_ELEMENT(player_data, player_datum,
                   player_index_from_unit_index(unit_index))->local_player_index != -1)
        {
            equipment_handle_pickup(equipment_index);
        }
        object_delete(equipment_index);
        return 1;
    }
    return 0;
}
