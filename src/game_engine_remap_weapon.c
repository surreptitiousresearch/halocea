/* game_engine_remap_weapon @0x837487E8 — translate a weapon definition index into the one the active
 * game variant should spawn, per the variant's weapon_set. Each weapon_set selects a remap table keyed
 * by the weapon's list-index; the chosen list slot indexes global_game_globals->weapon_list to yield
 * the final definition index (16-byte entries, remapped index at +12). Ball/flag/-1 are passthrough,
 * and classic mode on an original map filters out the flamethrower and plasma cannon (fuel rod gun),
 * which do not exist in the original maps.
 *
 * v1 (r6) is the original weapon_definition_index, returned by the passthrough path. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"
#include "headers/game_variant.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/weapon_list_index.h"

extern int weapon_definition_index_to_list_index(int weapon_definition_index);

int game_engine_remap_weapon(int weapon_definition_index)
{
    int list_index = weapon_definition_index_to_list_index(weapon_definition_index);
    int remapped = list_index;

    if ( gGameEngineRunningOriginalMap
        && (global_variant.universal_variant.flags & (1u << _game_mode_classic_bit)) != 0
        && (list_index == _weapon_list_flamethrower || list_index == _weapon_list_plasma_cannon) )
        return -1;

    if ( list_index == _weapon_list_ball || list_index == _weapon_list_flag || list_index == -1 )
        return weapon_definition_index;

    switch ( global_variant.universal_variant.weapon_set )
    {
        case _game_engine_weapons_pistols:
            switch ( list_index )
            {
                case _weapon_list_needler: case _weapon_list_pistol:
                case _weapon_list_plasma_rifle: case _weapon_list_rocket_launcher:
                case _weapon_list_plasma_cannon: case _weapon_list_mp_needler:
                    remapped = _weapon_list_plasma_pistol; break;
                default: remapped = _weapon_list_pistol; break;
            }
            break;
        case _game_engine_weapons_assault_rifles:
            switch ( list_index )
            {
                case _weapon_list_needler: case _weapon_list_pistol:
                case _weapon_list_plasma_rifle: case _weapon_list_rocket_launcher:
                case _weapon_list_plasma_cannon: case _weapon_list_mp_needler:
                    remapped = _weapon_list_plasma_rifle; break;
                default: remapped = _weapon_list_assault_rifle; break;
            }
            break;
        case _game_engine_weapons_plasma_weapons:
            remapped = (list_index >= _weapon_list_needler
                        && (list_index <= _weapon_list_plasma_pistol || list_index == _weapon_list_mp_needler))
                       ? _weapon_list_plasma_pistol : _weapon_list_plasma_rifle;
            break;
        case _game_engine_weapons_sniping:
            if ( list_index != _weapon_list_pistol && list_index != _weapon_list_sniper_rifle )
                remapped = _weapon_list_sniper_rifle;
            break;
        case _game_engine_weapons_no_sniping:
            if ( list_index == _weapon_list_pistol )
                remapped = _weapon_list_assault_rifle;
            else if ( list_index == _weapon_list_sniper_rifle )
                remapped = _weapon_list_shotgun;
            break;
        case _game_engine_weapons_rocket_launchers:
            remapped = _weapon_list_rocket_launcher;
            break;
        case _game_engine_weapons_shotguns:
            remapped = _weapon_list_shotgun;
            break;
        case _game_engine_weapons_short_range:
            switch ( list_index )
            {
                case _weapon_list_flamethrower: case _weapon_list_rocket_launcher:
                case _weapon_list_plasma_cannon:
                    remapped = _weapon_list_flamethrower; break;
                case _weapon_list_pistol: case _weapon_list_shotgun:
                    remapped = _weapon_list_shotgun; break;
                case _weapon_list_plasma_rifle: case _weapon_list_sniper_rifle:
                    remapped = _weapon_list_plasma_rifle; break;
                default: break;
            }
            break;
        case _game_engine_weapons_human:
            switch ( list_index )
            {
                case _weapon_list_needler: case _weapon_list_plasma_rifle:
                case _weapon_list_mp_needler:
                    remapped = _weapon_list_assault_rifle; break;
                case _weapon_list_plasma_pistol: remapped = _weapon_list_pistol; break;
                case _weapon_list_plasma_cannon: remapped = _weapon_list_rocket_launcher; break;
                default: break;
            }
            break;
        case _game_engine_weapons_covenant:
            switch ( list_index )
            {
                case _weapon_list_assault_rifle: case _weapon_list_flamethrower:
                case _weapon_list_sniper_rifle:
                    remapped = _weapon_list_plasma_rifle; break;
                case _weapon_list_needler: case _weapon_list_shotgun:
                    remapped = _weapon_list_mp_needler; break;
                case _weapon_list_pistol: remapped = _weapon_list_plasma_pistol; break;
                case _weapon_list_rocket_launcher: remapped = _weapon_list_plasma_cannon; break;
                default: break;
            }
            break;
        case _game_engine_weapons_classic:
            if ( list_index == _weapon_list_flamethrower )
                remapped = gGameEngineRunningOriginalMap == 0 ? _weapon_list_shotgun : -1;
            else if ( list_index == _weapon_list_plasma_cannon )
                remapped = gGameEngineRunningOriginalMap == 0 ? _weapon_list_rocket_launcher : -1;
            break;
        case _game_engine_weapons_heavy:
            switch ( list_index )
            {
                case _weapon_list_assault_rifle: case _weapon_list_pistol:
                case _weapon_list_rocket_launcher: case _weapon_list_sniper_rifle:
                    remapped = _weapon_list_rocket_launcher; break;
                case _weapon_list_flamethrower: case _weapon_list_needler:
                case _weapon_list_shotgun: case _weapon_list_mp_needler:
                    remapped = _weapon_list_flamethrower; break;
                case _weapon_list_plasma_pistol: case _weapon_list_plasma_rifle:
                case _weapon_list_plasma_cannon:
                    remapped = _weapon_list_plasma_cannon; break;
                default: break;
            }
            break;
        default:
            break;
    }

    if ( remapped == -1 )
        return -1;
    return ((tag_reference *)global_game_globals->weapon_list.address)[remapped].index;
}
