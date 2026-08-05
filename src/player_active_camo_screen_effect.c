/* player_active_camo_screen_effect @0x836AA278 — flash the active-camo pickup screen effect for a
 * local player. No-op for an invalid player or one with no local-player slot (+2 == 0xFFFF). The flash
 * parameters are pooled rodata constants (resolved and inlined here): a cyan (G=B... R=G=0.35) flash,
 * priority 2, type 2, fade function 0, duration 2.0, max intensity 1.0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/screen_flash_definition.h"
#include "headers/screen_flash_priority.h"
#include "headers/screen_flash_type.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int value, unsigned int size);
extern void player_effect_screen_flash(int player_index, const screen_flash_definition *screen_flash, float scale);

void player_active_camo_screen_effect(int player_index)
{
    if (player_index == -1
        || (uint16_t)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index == 0xFFFF)
        return;

    screen_flash_definition flash;
    memset(&flash.priority, 0, 0x36u);
    flash.priority = _screen_flash_high_priority;
    flash.duration = 2.0f;
    flash.fade_function = 0;
    flash.max_intensity = 1.0f;
    flash.type = _screen_flash_type_darken;
    flash.zero_scale_factor = 0.0f;
    flash.screen_flash_color.n[0] = 0.0f;        /* alpha */
    flash.screen_flash_color.n[1] = 0.34999999f; /* red */
    flash.screen_flash_color.n[2] = 0.34999999f; /* green */
    flash.screen_flash_color.n[3] = 0.0f;        /* blue */
    player_effect_screen_flash(player_index, &flash, 1.0f);
}
