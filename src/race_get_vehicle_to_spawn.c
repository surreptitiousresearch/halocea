/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* race_get_vehicle_to_spawn @0x83829E58 — pick which vehicle object (by remapped index into the scenario
 * globals' vehicle-remapper table) should occupy race spawn slot `vehicle_number`, based on the active game
 * variant's packed vehicle_set bitfield (top 4 bits = variant_type, an enum of built-in race vehicle-set
 * presets). Types 2-5 are simple fixed layouts gated only by `vehicle_number`'s range; type 0 is a small
 * fixed layout with its own per-slot mapping; type 8 rotates through warthog/ghost/rocket-warthog/tank in
 * that priority order, each gated by a per-round spawn limit extracted from vehicle_set and a
 * race_globals-tracked spawn counter (falling through to the next vehicle type whenever a type's slot is
 * exhausted or the remapper table returned no vehicle for it); types 1, 6, 7, and anything > 8 always
 * return -1 (no vehicle).
 *
 * The remapper table is the multiplayer_information vehicles tag block: the old raw int-view indices
 * (3/7/11/23) were tag_reference slots' .index members (+12 of each 16-byte entry) — slot order per
 * DB enum multiplayer_vehicle_index (warthog/ghost/scorpion/rocket_warthog). */

#include "headers/game_globals_tag.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_variant.h"
#include "headers/race_globals.h"
#include "headers/game_engine_vehicles.h"
#include "headers/tag_reference.h"
#include "headers/multiplayer_vehicle_index.h"
#include "headers/blam_data_globals.h"

extern game_variant *game_engine_get_variant(void);

int race_get_vehicle_to_spawn(int vehicle_number)
{
    int vehicle_index = -1;
    tag_reference *remapper_table = (tag_reference *)((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->vehicles.address;
    unsigned int variant_type = game_engine_get_variant()->universal_variant.vehicle_set.__s1.preset;

    if ( variant_type > _game_engine_vehicles_custom || variant_type == _game_engine_vehicles_none )
        return vehicle_index;

    switch ( variant_type )
    {
    case _game_engine_vehicles_warthog:
        return vehicle_number < 4 ? remapper_table[_multiplayer_vehicle_warthog].index : -1;
    case _game_engine_vehicles_ghost:
        return vehicle_number < 8 ? remapper_table[_multiplayer_vehicle_ghost].index : -1;
    case _game_engine_vehicles_tank:
        return vehicle_number < 4 ? remapper_table[_multiplayer_vehicle_scorpion].index : -1;
    case _game_engine_vehicles_rwarthog:
        return vehicle_number < 4 ? remapper_table[_multiplayer_vehicle_rocket_warthog].index : -1;
    }

    if ( variant_type == _game_engine_vehicles_banshee || variant_type == _game_engine_vehicles_turrets )
        return vehicle_index;

    if ( variant_type == _game_engine_vehicles_default )
    {
        if ( vehicle_number == 0 )
            return remapper_table[_multiplayer_vehicle_warthog].index;
        if ( vehicle_number == 1 )
            return remapper_table[_multiplayer_vehicle_scorpion].index;
        return vehicle_number < 6 ? remapper_table[_multiplayer_vehicle_ghost].index : vehicle_index;
    }

    game_variant *variant = game_engine_get_variant();
    vehicle_options_s vehicle_set = variant->universal_variant.vehicle_set;

    if ( race_globals.number_of_spawned_warthogs < vehicle_set.__s1.warthogs )
    {
        vehicle_index = remapper_table[_multiplayer_vehicle_warthog].index;
        ++race_globals.number_of_spawned_warthogs;
    }
    if ( vehicle_index == -1 )
    {
        if ( race_globals.number_of_spawned_ghosts < vehicle_set.__s1.ghosts )
        {
            vehicle_index = remapper_table[_multiplayer_vehicle_ghost].index;
            ++race_globals.number_of_spawned_ghosts;
        }
        if ( vehicle_index == -1 )
        {
            if ( race_globals.number_of_spawned_rocket_warthogs < vehicle_set.__s1.rwarthogs )
            {
                vehicle_index = remapper_table[_multiplayer_vehicle_rocket_warthog].index;
                ++race_globals.number_of_spawned_rocket_warthogs;
            }
            if ( vehicle_index == -1 && race_globals.number_of_spawned_tanks < vehicle_set.__s1.scorpions )
            {
                vehicle_index = remapper_table[_multiplayer_vehicle_scorpion].index;
                ++race_globals.number_of_spawned_tanks;
            }
        }
    }

    return vehicle_index;
}
