/* game_engine_predict_resources @0x8374A0A8 — precache the object/weapon resources the active netgame variant
 * will need: the selected vehicle set (or all vehicles for the "random/all" preset), the engine-specific
 * objective weapons (oddball/flag), and the standard multiplayer weapon set (remapped per variant).
 *
 * Deviation: the decompiler assembles the weapon table as 16 straight-line stores; rewritten as a copy loop
 * with sentinel overrides (identical result). The needler/ball/flag slots are excluded from prediction —
 * the needler is superseded by _weapon_list_mp_needler, and ball/flag are predicted per-engine above. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/tag_reference.h"
#include "headers/game_engine_vehicles.h"
#include "headers/weapon_list_index.h"
#include "headers/multiplayer_vehicle_index.h"


extern void object_definition_predict(int definition_index);
extern int game_engine_remap_weapon(int weapon_definition_index);

void game_engine_predict_resources(void)
{
    game_globals_multiplayer_information *vehicle_info = (game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address;
    tag_reference *vehicle_base = (tag_reference *)vehicle_info->vehicles.address;

    int selected_vehicle;
    unsigned int vehicle_preset = global_variant.universal_variant.vehicle_set.__s1.preset;
    /* deviation: decompiler form was `vehicle_preset - 2 > 5` (unsigned) — i.e. preset outside
     * [warthog..turrets]: default/none/custom all predict the full vehicle set */
    if ( vehicle_preset < _game_engine_vehicles_warthog || vehicle_preset > _game_engine_vehicles_turrets )
        goto predict_all_vehicles;
    switch ( vehicle_preset )
    {
        case _game_engine_vehicles_warthog:  selected_vehicle = vehicle_base[_multiplayer_vehicle_warthog].index; break;
        case _game_engine_vehicles_ghost:    selected_vehicle = vehicle_base[_multiplayer_vehicle_ghost].index; break;
        case _game_engine_vehicles_tank:     selected_vehicle = vehicle_base[_multiplayer_vehicle_scorpion].index; break;
        case _game_engine_vehicles_banshee:  selected_vehicle = vehicle_base[_multiplayer_vehicle_banshee].index; break;
        case _game_engine_vehicles_rwarthog:
        predict_all_vehicles:
            object_definition_predict(vehicle_base[_multiplayer_vehicle_warthog].index);
            object_definition_predict(vehicle_base[_multiplayer_vehicle_ghost].index);
            object_definition_predict(vehicle_base[_multiplayer_vehicle_scorpion].index);
            object_definition_predict(vehicle_base[_multiplayer_vehicle_banshee].index);
            object_definition_predict(vehicle_base[_multiplayer_vehicle_c_gun_turret].index);
            selected_vehicle = vehicle_base[_multiplayer_vehicle_rocket_warthog].index;
            break;
        default: selected_vehicle = vehicle_base[_multiplayer_vehicle_c_gun_turret].index; break;   /* turrets */
    }
    object_definition_predict(selected_vehicle);

    int game_engine_index = global_variant.game_engine_index;
    if ( game_engine_index == game_engine_oddball ) /* oddball */
    {
        object_definition_predict(((tag_reference *)global_game_globals->weapon_list.address)[_weapon_list_ball].index);
        game_engine_index = global_variant.game_engine_index;
    }
    if ( game_engine_index == game_engine_ctf ) /* ctf */
        object_definition_predict(((tag_reference *)global_game_globals->weapon_list.address)[_weapon_list_flag].index);

    int weapon_definitions[WEAPON_LIST_NUM_WEAPONS];
    tag_reference *weapons = (tag_reference *)global_game_globals->weapon_list.address;
    for ( int i = 0; i < WEAPON_LIST_NUM_WEAPONS; ++i )
        weapon_definitions[i] = weapons[i].index;
    weapon_definitions[_weapon_list_needler] = -1;   /* superseded by _weapon_list_mp_needler */
    weapon_definitions[_weapon_list_ball] = -1;
    weapon_definitions[_weapon_list_flag] = -1;

    for ( int i = 0; i < WEAPON_LIST_NUM_WEAPONS; ++i )
    {
        if ( weapon_definitions[i] != -1 )
            object_definition_predict(game_engine_remap_weapon(weapon_definitions[i]));
    }
}
