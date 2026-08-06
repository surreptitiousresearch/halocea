/* hcex_check_ach22 @0x83684120 — HCEX achievement 22 check ("levels\a50\a50" map): find the first
 * encounter (in iteration order, including inactive ones) whose scenario ai_encounters name is "covenant_area2", and if it currently has no living members and no enemy target, fire the
 * ACH22 player event. */

#include <stdint.h>
#include "../headers/scenario.h"
#include "../headers/encounter_iterator.h"
#include "../headers/encounter_datum.h"
#include "../headers/encounter_definition.h"

extern "C" scenario *global_scenario;

extern "C" char *main_get_map_name(void);
extern "C" int strcmp(const char *a, const char *b);
extern "C" void hcex_fire_plr_event(const char *event_name, int player_identifier); /* DEVIATION: slot 1 is int, not short -- def src/hcex/hcex_fire_plr_event.cpp + prologue stw r4 @0x823E4730 */

extern "C" void hcex_check_ach22(void)
{
    const char *map_name = main_get_map_name();

    if ( !strcmp(map_name, "levels\\a50\\a50") )
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
                    &((const encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)iterator.index];
                if ( !strcmp(enc_def->name, "covenant_area2") )
                    break;

                encounter = encounter_iterator_next(&iterator);
                if ( !encounter )
                    return;
            }

            if ( !encounter->current_count && !encounter->enemy_target )
                hcex_fire_plr_event("ACH22", -1);
        }
    }
}
