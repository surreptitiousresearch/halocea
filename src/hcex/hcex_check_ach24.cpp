/* hcex_check_ach24 @0x83683FC0 — HCEX achievement 24 check ("b30" map, Heroic or above): find the first
 * encounter (in iteration order, including inactive ones) whose scenario ai_encounters name is "beach_lz_marine", and if its current member count still equals its original count (nobody
 * lost), fire the ACH24 player event. */

#include <stdint.h>
#include "../headers/scenario.h"
#include "../headers/encounter_iterator.h"
#include "../headers/encounter_datum.h"
#include "../headers/encounter_definition.h"
#include "../headers/game_difficulty.h"

extern scenario *global_scenario;

extern char *main_get_map_name(void);
extern int16_t game_difficulty_level_get(void);
extern char *strstr(const char *string, const char *substring);
extern int strcmp(const char *a, const char *b);
extern void hcex_fire_plr_event(const char *event_name, short player_identifier);

extern "C" void hcex_check_ach24(void)
{
    const char *map_name = main_get_map_name();

    if ( strstr(map_name, "b30") && game_difficulty_level_get() >= game_difficulty_level_hard )
    {
        encounter_iterator iterator;
        encounter_iterator_new(&iterator, 0);
        encounter_datum *encounter = encounter_iterator_next(&iterator);

        if ( encounter )
        {
            while ( 1 )
            {
                /* typed encounter_definition walk (was a manual stride-176 byte cursor) */
                const encounter_definition *enc_def =
                    &((const encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)iterator.index];
                if ( !strcmp(enc_def->name, "beach_lz_marine") )
                    break;

                encounter = encounter_iterator_next(&iterator);
                if ( !encounter )
                    return;
            }

            if ( encounter->original_count == encounter->current_count )
                hcex_fire_plr_event("ACH24", -1);
        }
    }
}
