/* encounter_create @0x8370DEF8 — populate an encounter's squads with actors when the map is loaded (or when
 * an encounter is (re)spawned). When both desired_platoon_index and desired_squad_index are -1 every squad in
 * the encounter is filled; otherwise only the squad matching the requested platoon/squad is filled.
 *
 * The per-squad actor count is chosen by difficulty, the actor type is resolved through the actor palette ->
 * actor variant -> actor definition chain, and certain squads get a forced initial actor variant (the
 * v15/initial_variant codes: 100/101/12/11 and the random 100..101). */

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

/* Scenario encounter definition layout (stride 176): +128 squad_count, +132 squads address (squad stride 232).
 * Squad layout: +32 actor_type palette index, +34 platoon index, +44 initial spawn state,
 * +124 normal-difficulty count, +126 legendary-difficulty count. */

extern int16_t squad_get_actor_type(squad_definition *squad_definition);
void encounter_create(int encounter_index, int16_t desired_platoon_index, int16_t desired_squad_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    int encounter_index_u16 = (unsigned __int16)encounter_index;
    int desired_platoon = desired_platoon_index;
    encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (unsigned __int16)encounter_index;

    /* match_any_squad is true only when neither a platoon nor a squad was requested. */
    unsigned __int8 match_any_squad;
    if ( desired_platoon_index != -1 || (match_any_squad = 1, desired_squad_index != -1) )
        match_any_squad = 0;

    int squad_index = 0;
    if ( encounter_def->squads.count > 0 )
    {
        int squad_loop_index = 0;
        squad_definition *squad;

        while ( 1 )
        {
            squad = (squad_definition *)encounter_def->squads.address + squad_loop_index;
            if ( match_any_squad )
                break;
            if ( squad_loop_index == desired_squad_index )
                break;
            int squad_platoon_index = squad->platoon_index;
            if ( squad_platoon_index != -1 && squad_platoon_index == desired_platoon )
                break;

next_squad:
            squad_index = (__int16)(squad_loop_index + 1);
            squad_loop_index = squad_index;
            if ( squad_index >= encounter_def->squads.count )
                goto finished;
        }

        /* --- process the selected squad --- */
        __int16 initial_variant = 0;
        __int16 actor_count = 0;
        __int16 difficulty = game_difficulty_level_get();
        if ( (unsigned int)difficulty <= game_difficulty_level_impossible )
        {
            if ( difficulty != game_difficulty_level_normal || !difficulty )
            {
                if ( difficulty == game_difficulty_level_hard )       /* heroic: average of normal and legendary counts */
                {
                    actor_count = (squad->max_count + squad->min_count) / 2;
                    goto have_count;
                }
                if ( difficulty )            /* legendary */
                {
                    actor_count = squad->max_count;
                    goto have_count;
                }
            }
            actor_count = squad->min_count; /* easy and normal */
        }

have_count:
        ;
        /* DEVIATION: inlined copy of squad_get_actor_type@0x8370A838 (0 xrefs) collapsed to call; args verbatim, no folding. */
        /* DEVIATION: verbatim inlined copy of squad_get_actor_type@0x8370A838 (zero-xref donor) — collapsed to a
         * direct call. Single squad_definition* argument, no NULL-guard/index folding was needed. */
        __int16 actor_type = squad_get_actor_type(squad);

        unsigned int unique_leader_type = (unsigned __int16)squad->unique_leader_type;  /* prior name "spawn_state" was a guess */
        if ( unique_leader_type >= NUMBER_OF_UNIQUE_LEADER_TYPES || unique_leader_type == _leader_none )
            goto place_actors;
        if ( unique_leader_type == _leader_random )
            goto randomize_variant;
        if ( unique_leader_type == _leader_sergeant_johnson && squad->unique_leader_type )
        {
            if ( actor_type == _actor_type_marine )
                initial_variant = 100;
            goto place_actors;
        }
        if ( squad->unique_leader_type ) /* unique_leader_type == _leader_sergeant_lehto */
        {
            if ( actor_type == _actor_type_marine )
                initial_variant = 101;
            goto place_actors;
        }

        /* unique_leader_type == _leader_normal */
        {
            encounter_datum *encounter =
                DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index_u16);
            int should_randomize = 0;
            if ( actor_type == _actor_type_marine )
            {
                __int16 current_count;
                int threshold;
                /* recovered: ((__int16 *)encounter)[14] -> unique_leader_count, [12] -> original_count */
                if ( encounter->unique_leader_count )
                {
                    if ( encounter->unique_leader_count != 1 )
                        goto check_randomize;
                    current_count = encounter->original_count;
                    threshold = 10;
                }
                else
                {
                    current_count = encounter->original_count;
                    threshold = 4;
                }
                should_randomize = current_count + actor_count >= threshold;
            }
check_randomize:
            if ( should_randomize )
                goto randomize_variant;
        }

place_actors:
        if ( !desired_squad_index && desired_platoon == -1 )
        {
            /* Named special-case encounters get a fixed initial variant. */
            if ( strcmp(encounter_def->name, "cryo_tech") == 0 )
                initial_variant = 12;
            if ( strcmp(encounter_def->name, "fetch") == 0 )
                initial_variant = 11;
        }
        if ( actor_count > 0 )
        {
            int placed = 0;
            do
            {
                encounter_place_actor(encounter_index, squad_index, initial_variant, 0);
                initial_variant = 0; /* only the first actor of a squad carries the forced variant */
                placed = (__int16)(placed + 1);
            }
            while ( placed < actor_count );
        }
        goto next_squad;

randomize_variant:
        if ( actor_type == _actor_type_marine )
        {
            unsigned int *seed = get_global_random_seed_address();
            initial_variant = seed_random_range(seed, 0, 2) + 100;
        }
        goto place_actors;
    }

finished:
    encounter_update_status(encounter_index);
    encounters_test_activation();
}
