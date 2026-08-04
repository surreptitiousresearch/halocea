/* game_engine_remap_equipment @0x837485E0 — translate an equipment definition index into the one the
 * active game variant should actually spawn, honouring the variant's weapon-set selection, the
 * "no equipment" variant flag, and randomized-pickup rules.
 *
 * Equipment whose weapon list-index is _weapon_list_frag_grenade or _weapon_list_plasma_grenade is remapped by weapon_set;
 * other equipment is filtered out when its definition's powerup type conflicts with the variant
 * flags. A passthrough returns the original index. Verified prologue at 0x837485F0: r6 = equipment
 * definition data pointer (or 0 when index == -1), r5 = the passthrough index. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_reference.h"
#include "headers/game_engine_flags.h"
#include "headers/game_globals_tag.h"
#include "headers/game_variant.h"
#include "headers/game_engine_globals.h"
#include "headers/equipment_definition.h"
#include "headers/game_engine_weapons.h"
#include "headers/equipment_powerup_type.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/weapon_list_index.h"

extern int weapon_definition_index_to_list_index(int weapon_definition_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

extern unsigned int game_engine_infinite_grenades_internal(void);
int game_engine_remap_equipment(int equipment_definition_index)
{
    equipment_definition *definition = (equipment_definition_index == -1)
        ? 0
        : TAG_GET(equipment_definition, equipment_definition_index);

    int list_index = weapon_definition_index_to_list_index(equipment_definition_index);

    if ( list_index != _weapon_list_frag_grenade && list_index != _weapon_list_plasma_grenade )
    {
        if ( definition )
        {
            int powerup_type = definition->equipment.powerup_type;
            if ( powerup_type == _powerup_type_over_shield )
            {
                if ( (global_variant.universal_variant.flags & (1u << _no_shields_bit)) != 0 )
                    return -1;
            }
            else if ( powerup_type == _powerup_type_active_camouflage && (global_variant.universal_variant.flags & (1u << _always_invisible_bit)) != 0 )
            {
                return -1;
            }
        }
        return equipment_definition_index;
    }

    /* grenade remap by weapon set */
    int remapped = list_index;
    switch ( global_variant.universal_variant.weapon_set )
    {
        case _game_engine_weapons_plasma_weapons:
        case _game_engine_weapons_covenant:   remapped = _weapon_list_plasma_grenade; break;
        case _game_engine_weapons_human:      remapped = _weapon_list_frag_grenade; break;
        case _game_engine_weapons_no_grenades: remapped = -1; break;
        default:  break;
    }

    /* DEVIATION: randomize computation is a verbatim-inlined copy of zero-xref game_engine_infinite_grenades_internal@0x83748428, reading the globals directly (not cached); collapsed to a direct call */
    unsigned char randomize = game_engine_infinite_grenades_internal();
    if ( randomize )
        remapped = -1;

    if ( (game_engine_globals.flags & (1u << _game_engine_9_or_more_players_bit)) != 0 )
    {
        if ( real_seed_random(get_global_random_seed_address()) > 0.30000001 )
            remapped = -1;
    }
    else if ( (game_engine_globals.flags & (1u << _game_engine_5_or_more_players_bit)) != 0 )
    {
        if ( real_seed_random(get_global_random_seed_address()) > 0.55000001 )
            remapped = -1;
    }

    if ( remapped == -1 )
        return -1;
    return ((tag_reference *)global_game_globals->weapon_list.address)[remapped].index;
}
