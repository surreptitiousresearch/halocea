/* player_over_shield_screen_effect @0x836AA1A0 — flash the overshield pickup screen effect for a
 * local player. No-op for an invalid player or one with no local-player slot (+2 == 0xFFFF). The flash
 * parameters are pooled rodata constants (resolved and inlined here): a purple (R=B=0.8) full-screen
 * flash, priority 2, type 5, fade function 0, duration 2.0, max intensity 1.0. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/screen_flash_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/render_screen_flash_type.h"
#include "headers/screen_flash_priority.h"

extern void *memset(void *dst, int value, unsigned int size);
extern void player_effect_screen_flash(int player_index, const screen_flash_definition *screen_flash, float scale);

void player_over_shield_screen_effect(int player_index)
{
    if (player_index == -1
        || (unsigned __int16)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index == 0xFFFF)
        return;

    screen_flash_definition flash;
    memset(&flash.priority, 0, 0x36u);
    flash.priority = _screen_flash_high_priority;
    flash.duration = 2.0f;
    flash.fade_function = 0;
    flash.max_intensity = 1.0f;
    flash.type = _render_screen_flash_type_invert;
    flash.zero_scale_factor = 0.0f;
    flash.screen_flash_color.n[0] = 0.0f;        /* alpha */
    flash.screen_flash_color.n[1] = 0.80000001f; /* red */
    flash.screen_flash_color.n[2] = 0.0f;        /* green */
    flash.screen_flash_color.n[3] = 0.80000001f; /* blue */
    player_effect_screen_flash(player_index, &flash, 1.0f);
}
