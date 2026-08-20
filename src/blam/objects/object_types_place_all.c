/* object_types_place_all @0x83702EC0 — place every scenario-placed object across all 12 object types when a map
 * loads (skipped entirely in the editor). Vehicles (type 1) are special-cased: in a netgame the vehicle
 * remapper decides which vehicles spawn and how they are substituted per game variant.
 *
 * Object types are skipped when bit i is set in object_mask_remove_on_bsp_switch (576 = bits 6 and 9 —
 * scenery and light_fixture, placed per-BSP by object_types_place_objects), and on a network client
 * (game_connection() == 1, cmpwi @0x83702FD4/0x83702FE4) the vehicle type (i == 1) is skipped as well —
 * a client receives its vehicles from the server rather than placing them itself. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/object_type_definition.h"
#include "headers/scenario_object_datum.h"
#include "headers/network_game_data.h"
#include "headers/tag_block.h"
#include "headers/scenario_vehicle_datum.h"
#include "headers/scenario_object_palette_entry.h"
#include "headers/game_engine_type.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/tag_groups.h"


extern uint8_t game_in_editor(void);
extern network_game_data *network_game_get_game(void);
extern int16_t game_connection(void);
extern void vehicle_remapper_init(void);
extern void vehicle_remapper_process(void);
extern uint8_t vehicle_remapper_should_spawn(int vehicle_ref_index, unsigned int scenario_datum_index);
extern void vehicle_remapper_add_vehicle(unsigned int team, int vehicle_ref_index, unsigned int scenario_datum_index, uint16_t spawn_flags);
extern int object_new_from_scenario(scenario_object_datum *scenario_object, tag_block *palette);
extern void vehicle_set_scenario_datum_index(int vehicle_index, int16_t scenario_datum_index);
extern void objects_garbage_collection(void);
extern void object_types_place_objects(uint8_t place);

void object_types_place_all(scenario *scenario_ptr)
{
    if ( game_in_editor() )
        return;

    /* race ("king of the hill"-style engine 5) suppresses the vehicle remapper substitution */
    int is_race_variant = 0;
    network_game_data *game = network_game_get_game();
    if ( game )
        is_race_variant = game->variant.game_engine_index == game_engine_race;

    if ( !is_race_variant )
    {
        vehicle_remapper_init();
        if ( game_connection() == _game_connection_network_server ) /* network SERVER (cmpwi r11,2 @0x83702F24): register the scenario vehicles with the remapper */
        {
            const tag_block *placements = (const tag_block *)((char *)scenario_ptr + object_type_definitions[1]->placement_tag_block_offset);
            int placement_element_size = object_type_definitions[1]->placement_tag_block_element_size;
            tag_block *vehicle_palette = (tag_block *)((char *)scenario_ptr + object_type_definitions[1]->palette_tag_block_offset);
            for ( int i = 0; i < placements->count; i = (int16_t)(i + 1) )
            {
                scenario_vehicle_datum *placement =
                    (scenario_vehicle_datum *)tag_block_get_element_with_size(placements, i, placement_element_size);
                int palette_index = placement->object.palette_entry_index;
                if ( palette_index != -1 )
                    /* DB: +88 is multiplayer_team_index, +90 multiplayer_spawn_flags (extern arg names kept) */
                    vehicle_remapper_add_vehicle(placement->multiplayer_team_index,
                                                 ((scenario_object_palette_entry *)vehicle_palette->address)[palette_index].reference.index,
                                                 i,
                                                 placement->multiplayer_spawn_flags);
            }
        }
        vehicle_remapper_process();
    }

    for ( int i = 0; i < 12; i = (int16_t)(i + 1) )
    {
        /* on a network client (connection 1) skip vehicles (i==1); always skip scenery+light_fixture
         * (object_mask_remove_on_bsp_switch = 576) — placed per-BSP by object_types_place_objects */
        if ( (game_connection() != _game_connection_network_client || (game_connection() == _game_connection_network_client && i != 1)) && ((1 << i) & object_mask_remove_on_bsp_switch) == 0 )
        {
            object_type_definition *type = object_type_definitions[i];
            if ( (uint16_t)type->placement_tag_block_offset != 0xFFFF
              && (uint16_t)type->palette_tag_block_offset != 0xFFFF )
            {
                int16_t last_index = 0;
                const tag_block *placements = (const tag_block *)((char *)scenario_ptr + type->placement_tag_block_offset);
                int placement_element_size = type->placement_tag_block_element_size;
                tag_block *palette = (tag_block *)((char *)scenario_ptr + type->palette_tag_block_offset);
                for ( unsigned int i2 = 0; (int)i2 < placements->count; )
                {
                    scenario_object_datum *datum = (scenario_object_datum *)tag_block_get_element_with_size(placements, i2, placement_element_size);
                    if ( (i != 1
                          || (!is_race_variant
                              && vehicle_remapper_should_spawn(
                                     ((scenario_object_palette_entry *)palette->address)[datum->palette_entry_index].reference.index, i2)))
                         && datum )
                    {
                        global_object_scenario_index = i2 + 1;
                        int object_index = object_new_from_scenario(datum, palette);
                        global_object_scenario_index = 0;
                        if ( object_index != -1 && i == 1 )
                            vehicle_set_scenario_datum_index(object_index, last_index);
                        objects_garbage_collection();
                    }
                    last_index = (int16_t)(i2 + 1);
                    i2 = last_index;
                }
            }
        }
    }
    object_types_place_objects(1u);
}
