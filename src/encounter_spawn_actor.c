/* encounter_spawn_actor @0x8370BE60 — place (spawn) one actor of a squad into its encounter, bumping the
 * encounter's and squad's live counts, decrementing the squad's remaining respawns, and rolling fresh
 * encounter- and squad-level respawn delay timers from the definition's delay ranges. Always returns 0. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/data_array.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_place_actor(int encounter_index, int16_t squad_index, int16_t variant_number, uint8_t spawn);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

/* DEVIATION (2026-08-12, #141): encounter_index is `int`, refuting the #134 KEEP/NARROW-UNSIGNED
 * verdict that left it `uint16_t`. That verdict rested on ai_scripting_spawn_actor's clrlwi r3,r3,16
 * @0x8376F8FC being a truncation "at the call boundary" — but that instruction extracts a field from a
 * PACKED haloscript ai reference (srwi r10,r3,30 @0x8376F8F8 takes the scope tag from bits 31-30,
 * extrwi r11,r7,8,8 @0x8376F904 the squad selector), so it is required whatever width the callee
 * declares; it discriminates nothing. It is also not the only caller. encounter_update_respawn
 * @0x8370C020 — itself `int encounter_index` — saves its parameter UNMASKED (mr r21,r3 @0x8370C034;
 * its clrlwi r11,r3,16 @0x8370C030 is the encounter_data subscript) and passes that whole word at both
 * of its call sites (mr r3,r21 @0x8370C108 -> bl @0x8370C10C, and @0x8370C238 -> bl @0x8370C23C).
 *
 * The mask does not hold through the call tree either. This body forwards r3 untouched to
 * encounter_place_actor (bl @0x8370BE9C; r3 is never rewritten after the prologue, only copied to r31);
 * encounter_place_actor saves it unmasked (mr r30,r3 @0x8370BD48, its clrlwi @0x8370BD18 being the
 * 0xB0 scenario subscript) and forwards it twice more — mr r3,r30 -> bl ai_consider_major_upgrade
 * @0x8370BE14-18 and mr r4,r30 -> bl actor_place @0x8370BE34-38; and actor_place tests that taint at
 * FULL WIDTH, cmpwi cr6,r27,-1 @0x837216D8 (r27 = mr r27,r4 @0x83721610), a compare a zero-extended
 * uint16_t can never satisfy. All three callees already declare `int encounter_index`; only
 * funcs.prototype still says `unsigned __int16`. */
uint8_t encounter_spawn_actor(int encounter_index, int16_t squad_index)
{
    if ( ai_globals->ai_initialized_for_map && encounter_place_actor(encounter_index, squad_index, 0, 1u) )
    {
        encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
        /* the scenario subscript reuses the SAME masked index the datum subscript computed
         * (clrlwi r7,r31,16 @0x8370BEB0 feeding both mulli 0x6C @0x8370BEB8 and mulli 0xB0 @0x8370BED4) */
        encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_index;
        squad_definition *squad_def = (squad_definition *)encounter_def->squads.address + squad_index;
        squad_datum *squad = &squad_array[(int16_t)(encounter->squad_base + squad_index)];

        ++encounter->current_count;
        ++squad->current_count;
        if ( squad_def->respawn_total_count > 0 )
            --squad->respawn_actors_left;

        float encounter_delay_max = encounter_def->respawn_time_upper_bound;
        float encounter_delay_min = encounter_def->respawn_time_lower_bound;
        unsigned int *seed = get_global_random_seed_address();
        encounter->respawn_delay_ticks = (int)(real_seed_random_range(seed, encounter_delay_min, encounter_delay_max) * 30.0f);

        float squad_delay_max = squad_def->respawn_time_upper_bound;
        float squad_delay_min = squad_def->respawn_time_lower_bound;
        unsigned int *squad_seed = get_global_random_seed_address();
        squad->respawn_delay_ticks = (int)(real_seed_random_range(squad_seed, squad_delay_min, squad_delay_max) * 30.0f);
    }
    return 0;
}
