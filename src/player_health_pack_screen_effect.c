/* player_health_pack_screen_effect @0x836AA350 — flash the health-pack pickup screen effect for a
 * local player. No-op for an invalid player or one with no local-player slot (+2 == 0xFFFF). A near-
 * white (0.917647) flash, priority 2, type tint, fade function 1, duration 2.0, max intensity 0.5. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/screen_flash_definition.h"
#include "headers/render_screen_flash_type.h"
#include "headers/blam_data_globals.h"
#include "headers/screen_flash_priority.h"

extern void *memset(void *dst, int value, unsigned int size);
extern void player_effect_screen_flash(int player_index, const screen_flash_definition *screen_flash, float scale);

void player_health_pack_screen_effect(int player_index)
{
    if (player_index == -1
        || (unsigned __int16)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index == 0xFFFF)
        return;

    screen_flash_definition flash;
    memset(&flash.priority, 0, 0x36u);
    flash.fade_function = 1;
    flash.priority = _screen_flash_high_priority;
    flash.screen_flash_color.n[0] = 1.0f;        /* alpha */
    flash.type = _render_screen_flash_type_tint;
    flash.duration = 2.0f;
    flash.max_intensity = 0.5f;
    flash.zero_scale_factor = 0.0f;
    flash.screen_flash_color.n[1] = 0.917647f;   /* red */
    flash.screen_flash_color.n[2] = 0.917647f;   /* green */
    flash.screen_flash_color.n[3] = 0.917647f;   /* blue */
    player_effect_screen_flash(player_index, &flash, 1.0f);
}
