/* hcex_check_ach31 @0x83683E78 — HCEX achievement 31 check ("c20" map, Legendary, speed run): award ACH31
 * when the current map is the "c20" level, the game is on Legendary (difficulty level 3), and the elapsed
 * game time is at most 54000 ticks (30 minutes at 30Hz). Fires the player event with plr_idx == -1. */

#include <stdint.h>
#include "../headers/game_difficulty.h"

extern char *main_get_map_name(void);
extern char *strstr(const char *haystack, const char *needle);
extern int16_t game_difficulty_level_get(void);
extern int game_time_get(void);
extern void hcex_fire_plr_event(const char *event_name, short player_identifier);

extern "C" void hcex_check_ach31(void)
{
    const char *map_name = main_get_map_name();
    if ( strstr(map_name, "c20") && game_difficulty_level_get() == game_difficulty_level_impossible && game_time_get() <= 54000 )
        hcex_fire_plr_event("ACH31", -1);
}
