/* hcex_update_screen_flash @0x8369E770 — refresh the global render window's screen-flash parameters
 * from the player-effect system for the camera being rendered. Translates the HCEX camera index to a
 * local player index and copies that player's current screen flash into global_window_parameters. */

#include <stdint.h>
#include "../headers/rasterizer_window_begin_parameters.h"
#include "../headers/render_screen_flash.h"

#include "headers/render_screen_flash.h"
extern "C" rasterizer_window_begin_parameters global_window_parameters;

extern "C" short hcex_render_player_index(int cam_idx);
extern "C" void player_effect_get_screen_flash(int16_t local_player_index, render_screen_flash *screen_flash);

extern "C" void hcex_update_screen_flash(int cam_idx)
{
    short local_player_index = hcex_render_player_index(cam_idx);
    player_effect_get_screen_flash(local_player_index, &global_window_parameters.screen_flash);
}
