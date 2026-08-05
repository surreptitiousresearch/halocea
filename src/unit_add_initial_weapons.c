/* unit_add_initial_weapons @0x836D3DF0 — spawn and add a unit's starting-equipment weapons: the unit
 * definition's initial-weapons tag_block (count at +728, 36-byte records at +732 with a weapon definition
 * index at each record's +12) is walked; for each defined entry, a new weapon object is placed at the
 * unit's origin and added to its inventory, unless the game engine is running and the unit already has
 * that weapon definition (in which case the newly spawned object is discarded), or the inventory add
 * itself fails (also discarded). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_initial_weapon.h"
#include "headers/global_tag_instances.h"
#include "headers/object_placement_data.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_running(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern uint8_t unit_has_weapon_definition_index(int unit_index, int weapon_definition_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void object_delete(int object_index);

void unit_add_initial_weapons(int unit_index)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    int initial_weapon_count = definition->unit.initial_weapons.count;
    if ( initial_weapon_count <= 0 )
        return;

    unit_initial_weapon *initial_weapons = (unit_initial_weapon *)definition->unit.initial_weapons.address;

    for ( int16_t i = 0; i < initial_weapon_count; i++ )
    {
        int weapon_definition_index = initial_weapons[i].reference.index;
        if ( weapon_definition_index == -1 )
            continue;

        object_placement_data placement;
        object_placement_data_new(&placement, weapon_definition_index, unit_index);
        int weapon_object_index = object_new(&placement);
        if ( weapon_object_index == -1 )
            continue;

        object_datum *weapon_object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum;

        if ( (game_engine_running() && unit_has_weapon_definition_index(unit_index, weapon_object->definition_index))
          || !unit_add_weapon_to_inventory(unit_index, weapon_object_index, _unit_add_weapon_normally) )
        {
            object_delete(weapon_object_index);
        }
    }
}
