/* encounter_create @0x8370DEF8 — populate an encounter's squads with actors when the map is loaded (or when
 * an encounter is (re)spawned). When both desired_platoon_index and desired_squad_index are -1 every squad in
 * the encounter is filled; otherwise only the squad matching the requested platoon/squad is filled.
 *
 * The per-squad actor count is chosen by difficulty, the actor type is resolved through the actor palette ->
 * actor variant -> actor definition chain, and certain squads get a forced initial actor variant.
 *
 * DEVIATION (control flow): the decompiler flattens this into six labels and a backward goto. Disasm
 * 0x8370E22C-0x8370E240 is a plain increment/compare back-edge to the loop head at 0x8370DF90, and both
 * dispatches (difficulty @0x8370DFD0, unique_leader_type @0x8370E088) are MSVC's bounds-check + mtctr/bdzf
 * dense-switch idiom, so they are restored as a `for` and two `switch`es. The conditional fall-through from
 * _leader_normal into _leader_random is real: 0x8370E11C branches past the switch when should_randomize is
 * clear and otherwise falls into the _leader_random body at 0x8370E120. */

#include <stdint.h>
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include <string.h>
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_palette_entry.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_type.h"
#include "headers/game_difficulty.h"
#include "headers/unique_leader_type.h"
#include "headers/blam_data_globals.h"


extern int16_t game_difficulty_level_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern uint8_t encounter_place_actor(int encounter_index, int16_t squad_index, int16_t variant_number, uint8_t spawn);
extern void encounter_update_status(int encounter_index);
extern void encounters_test_activation(void);
extern int16_t squad_get_actor_type(squad_definition *squad_definition);

void encounter_create(int encounter_index, int16_t desired_platoon_index, int16_t desired_squad_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    int encounter_index_u16 = (uint16_t)encounter_index;
    encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_index;

    /* match_any_squad is true only when neither a platoon nor a squad was requested. */
    uint8_t match_any_squad = (uint8_t)(desired_platoon_index == -1 && desired_squad_index == -1);

    /* DEVIATION: the difficulty switch below has no default arm, so the binary carries actor_count in from
     * an uninitialized frame slot (lhz r28, var_A0 @0x8370DF6C, hoisted out of the loop). Initialized to 0
     * here; unreachable in practice since game_difficulty_level_get() is bounds-checked to 0..3. */
    int16_t actor_count = 0;

    for ( int16_t squad_index = 0; squad_index < encounter_def->squads.count; ++squad_index )
    {
        squad_definition *squad = (squad_definition *)encounter_def->squads.address + squad_index;

        if ( !match_any_squad && squad_index != desired_squad_index )
        {
            int16_t squad_platoon_index = squad->platoon_index;
            if ( squad_platoon_index == -1 || squad_platoon_index != desired_platoon_index )
                continue;
        }

        int16_t initial_variant = 0;

        switch ( game_difficulty_level_get() )
        {
        case game_difficulty_level_easy:
        case game_difficulty_level_normal:
            actor_count = squad->min_count;
            break;
        case game_difficulty_level_hard:        /* heroic: average of normal and legendary counts */
            actor_count = (squad->max_count + squad->min_count) / 2;
            break;
        case game_difficulty_level_impossible:
            actor_count = squad->max_count;
            break;
        }

        /* DEVIATION: verbatim inlined copy of squad_get_actor_type@0x8370A838 (zero-xref donor) — collapsed
         * to a direct call. Single squad_definition* argument, no NULL-guard/index folding was needed. */
        int16_t actor_type = squad_get_actor_type(squad);

        switch ( (uint16_t)squad->unique_leader_type )
        {
        case _leader_normal:
            {
                int should_randomize = 0;
                if ( actor_type == _actor_type_marine )
                {
                    encounter_datum *encounter =
                        DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index_u16);
                    if ( encounter->unique_leader_count == 0 )
                        should_randomize = encounter->original_count + actor_count >= 4;
                    else if ( encounter->unique_leader_count == 1 )
                        should_randomize = encounter->original_count + actor_count >= 10;
                }
                if ( !should_randomize )
                    break;
            }
            /* fall through — see the DEVIATION note above */
        case _leader_random:
            if ( actor_type == _actor_type_marine )
            {
                uint32_t *seed = get_global_random_seed_address();
                initial_variant = seed_random_range(seed, 0, 2) + 100;
            }
            break;
        case _leader_sergeant_johnson:
            if ( actor_type == _actor_type_marine )
                initial_variant = 100;
            break;
        case _leader_sergeant_lehto:
            if ( actor_type == _actor_type_marine )
                initial_variant = 101;
            break;
        case _leader_none:
            break;
        }

        if ( desired_squad_index == 0 && desired_platoon_index == -1 )
        {
            /* Named special-case encounters get a fixed initial variant. */
            if ( strcmp(encounter_def->name, "cryo_tech") == 0 )
                initial_variant = 12;
            if ( strcmp(encounter_def->name, "fetch") == 0 )
                initial_variant = 11;
        }

        if ( actor_count > 0 )
        {
            int16_t placed = 0;
            do
            {
                encounter_place_actor(encounter_index, squad_index, initial_variant, 0);
                initial_variant = 0; /* only the first actor of a squad carries the forced variant */
                ++placed;
            }
            while ( placed < actor_count );
        }
    }

    encounter_update_status(encounter_index);
    encounters_test_activation();
}
