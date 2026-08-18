/* hcex_check_ach37 @0x836840B8 — hcex achievement bridge: award ACH37 when the current map is the "d40"
 * level (The Maw), the game is on Legendary (difficulty level 3), and the on-screen mission timer has run
 * at least 1800 ticks. Fires the player event with plr_idx == -1 (all/local player). */

#include <stdint.h>

extern "C" char *main_get_map_name(void);
extern "C" char *strstr(const char *haystack, const char *needle);
extern "C" int16_t game_difficulty_level_get(void);
extern "C" int halo_hud_get_timer_ticks(void);
extern "C" void hcex_fire_plr_event(const char *eventName, int plr_idx);

extern "C" void hcex_check_ach37(void)
{
    const char *map_name = main_get_map_name();
    if ( strstr(map_name, "d40") && game_difficulty_level_get() == 3 && halo_hud_get_timer_ticks() >= 1800 )
        hcex_fire_plr_event("ACH37", -1);
}
