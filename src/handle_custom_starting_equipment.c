/* handle_custom_starting_equipment @0x8374DA34 — apply the scenario's custom starting-equipment profile
 * (the first whose game-type list matches the active game engine type) to a unit. For each of the
 * profile's five weapon slots it spawns a random item from the slot's item collection and adds it to the
 * unit's inventory; the first accepted weapon is the held one (mode 2), the rest are stowed (mode 0), and
 * a duplicate weapon definition (after the first) is discarded. Finally the profile flags adjust the
 * caller's grenade counts: bit 0 clears both, bit 1 folds frag into plasma.
 *
 * Profile is scenario_starting_equipment: flags@0, game_type[4]@4, item_collection: tag_reference[6]@60
 * (the code uses the first 5, keying each spawn off item_collection[slot].index). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/netgame_starting_equipment_flag_flags.h"
#include "headers/scenario_starting_equipment.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_placement_data.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/blam_data_globals.h"


extern unsigned int game_engine_get_type(void);
extern uint8_t match_game_type(int game_engine_type, int count, const int16_t *game_type);
extern int random_item(int item_collection_index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern uint8_t unit_has_weapon_definition_index(int unit_index, int weapon_definition_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void object_delete(int object_index);

void handle_custom_starting_equipment(int unit_index, int *frag_count, int *plasma_count)
{
    int count = global_scenario->scenario_starting_equipment.count;
    if (count <= 0)
        return;

    scenario_starting_equipment *equipment =
        (scenario_starting_equipment *)global_scenario->scenario_starting_equipment.address;

    int index = 0;
    scenario_starting_equipment *profile;
    while (1)
    {
        profile = &equipment[index];
        if (match_game_type(game_engine_get_type(), 4, profile->game_type))
            break;
        ++index;
        if (index >= count)
            return;
    }

    char first_weapon = 1;
    for (int slot = 0; slot < 5; slot++)
    {
        int item_collection_index = profile->item_collection[slot].index;
        if (item_collection_index != -1)
        {
            int item_definition = random_item(item_collection_index);
            object_placement_data placement;
            object_placement_data_new(&placement, item_definition, -1);
            int object_index = object_new(&placement);
            if (object_index != -1)
            {
                int weapon_definition = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum->definition_index;
                if (first_weapon || !unit_has_weapon_definition_index(unit_index, weapon_definition))
                {
                    unit_add_weapon_to_inventory(unit_index, object_index,
                        first_weapon ? _unit_add_weapon_as_only_weapon : _unit_add_weapon_normally);
                    first_weapon = 0;
                }
                else
                {
                    object_delete(object_index);
                }
            }
        }
    }

    if ((profile->flags & (1u << _netgame_starting_equipment_flag_no_grenades_bit)) != 0)
    {
        *frag_count = 0;
        *plasma_count = 0;
    }
    if ((profile->flags & (1u << _netgame_starting_equipment_flag_plasma_greandes_bit)) != 0)
    {
        *plasma_count += *frag_count;
        *frag_count = 0;
    }
}
