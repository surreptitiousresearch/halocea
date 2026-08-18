/* game_engine_postspawn_player_update @0x8374DB88 — give a just-spawned player their starting grenades,
 * according to the active game engine and variant. First the engine's optional postspawn callback may veto
 * the whole update. Otherwise it starts from the globals' default frag/plasma counts (overridden by custom
 * starting equipment for biped units when the variant allows it, or by the per-mode maximum), then applies
 * the variant's weapon-set remap (all-plasma, all-frag, or none), clamps to the per-mode maximums, and
 * writes the two grenade counts into the unit object (unit.grenade_counts[0] frag, [1] plasma).
 *
 * The frag/plasma maxima are 1 when game_engine flag 0x8 is set, 2 when flag 0x4 is set, else the globals'
 * configured maxima. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_flags.h"
#include "headers/game_engine_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_grenade_type.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern void handle_custom_starting_equipment(int unit_index, int *frag_count, int *plasma_count);

extern unsigned int game_engine_infinite_grenades_internal(void);
void game_engine_postspawn_player_update(int player_index)
{
    if (!game_engine)
        return;

    uint8_t (*postspawn_player_update)(int) = game_engine->postspawn_player_update;
    char proceed = 1;
    if (postspawn_player_update)
        proceed = postspawn_player_update(player_index);
    if (!proceed)
        return;

    unsigned int flags /* DEVIATION: was char; binary caches the 32-bit word lwz @0x8374DBEC */ = game_engine_globals.flags;
    int16_t *grenades = (int16_t *)global_game_globals->grenades.address;
    int frag_maximum = grenades[0];
    int frag_count = grenades[1];
    int plasma_maximum = grenades[34];
    int plasma_count = grenades[35];

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    int custom_frag = grenades[1];
    int custom_plasma = grenades[35];

    if ((game_engine_globals.flags & (1u << _game_engine_9_or_more_players_bit)) != 0)
    {
        frag_maximum = 1;
        plasma_maximum = 1;
    }
    else if ((game_engine_globals.flags & (1u << _game_engine_5_or_more_players_bit)) != 0)
    {
        frag_maximum = 2;
        plasma_maximum = 2;
    }

    unsigned int variant_flags = global_variant.universal_variant.flags;
    if ((global_variant.universal_variant.flags & (1u << _generic_starting_equipment_bit)) == 0)
    {
        /* datum_role @+4, not object type: only locally-simulated units (master/autonomous) */
        int datum_role = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum))->object.datum_role;
        if (datum_role == _networked_datum_master || datum_role == _networked_datum_autonomous)
        {
            handle_custom_starting_equipment(unit_index, &custom_frag, &custom_plasma);
            frag_count = custom_frag;
            plasma_count = custom_plasma;
            flags = game_engine_globals.flags;
            variant_flags = global_variant.universal_variant.flags;
        }
    }

    /* DEVIATION: use_maximum computation is a verbatim-inlined copy of zero-xref game_engine_infinite_grenades_internal@0x83748428 read from local caches of game_engine_globals.flags/global_variant.universal_variant.flags that are unmodified since capture; collapsed to a direct call (donor's implicit-0-on-guard-fail matches use_maximum's initializer) */
    char use_maximum = game_engine_infinite_grenades_internal();
    if (use_maximum)
    {
        frag_count = frag_maximum;
        plasma_count = plasma_maximum;
    }

    if (unit_index == -1)
        return;

    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    if (unit->object.datum_role != _networked_datum_master
     && unit->object.datum_role != _networked_datum_autonomous)
        return;

    switch (global_variant.universal_variant.weapon_set)
    {
        case _game_engine_weapons_plasma_weapons:
        case _game_engine_weapons_covenant:
            plasma_count += frag_count;
            frag_count = 0;
            break;
        case _game_engine_weapons_human:
            frag_count += plasma_count;
            plasma_count = 0;
            break;
        case _game_engine_weapons_no_grenades:
        {
            char use_maximum_2 = 0;
            if ((flags & (1u << _game_engine_5_or_more_players_bit)) == 0)
                use_maximum_2 = (variant_flags >> _infinite_grenades_bit) & 1;
            if (!use_maximum_2)
            {
                frag_count = 0;
                plasma_count = 0;
            }
            break;
        }
        default:
            break;
    }

    if (frag_count > frag_maximum)
        frag_count = frag_maximum;
    if (plasma_count > plasma_maximum)
        plasma_count = plasma_maximum;
    unit->unit.grenade_counts[_unit_grenade_human_fragmentation] = (char)frag_count;
    unit->unit.grenade_counts[_unit_grenade_covenant_plasma] = (char)plasma_count;
}
