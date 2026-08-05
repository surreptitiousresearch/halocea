/* hud_draw_screen 0x83708DB8 — top-level HUD draw for the active local player, bracketed by the rasterizer HUD
 * begin/end pair. Draws player nameplates/team indicators, ticks the motion sensor for the primary (or co-op)
 * player, and — when the HUD is scripted-visible and the player is a living unit in a head/third-person view
 * other than dead/cutscene — renders the full weapon + unit interface, nav points, damage indicators, and
 * messaging. Otherwise it still plays unit sounds and updates messaging.
 *
 * DEVIATION: rasterizer_hud_begin/rasterizer_hud_end take no arguments (confirmed: both are no-arg thunks and
 * the disassembly sets up no r3 before the end calls); the decompiler's v2/v4 arguments are spurious r3 leftover.
 * DEVIATION: returns void — r3 at every blr is only rasterizer_hud_end's (attested void) leftover and the sole
 * caller interface_draw_screen @836A6BB8 ignores it; the int return was decompiler r3-status threading. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/hud_scripted_globals.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern int16_t local_player_get_next(int16_t local_player_index);
extern void rasterizer_hud_begin(void);
extern void rasterizer_hud_end(void);
extern uint8_t game_engine_running(void);
extern uint8_t game_engine_display_team_indicators(void);
extern uint8_t cinematic_in_progress(void);
extern uint8_t game_time_get_paused(void);
extern void hud_draw_players(void);
extern void motion_sensor_tick(void);
extern void hud_render_weapon_interface(player_datum *player);
extern void hud_show_action_response(int player_index);
extern void hud_play_unit_sounds(const player_datum *player, uint8_t show_hud);
extern void hud_render_unit_interface(player_datum *player);
extern void hud_render_nav_points(int16_t local_player_index);
extern void hud_render_damage_indicators(int16_t local_player_index);
extern void hud_messaging_update(int16_t local_player_index);

void hud_draw_screen(void)
{
    int player_index = local_player_get_player_index(render.local_player_index);
    int16_t perspective = director_get_perspective(render.local_player_index);
    rasterizer_hud_begin();
    if ( player_index == -1 )
    {
        rasterizer_hud_end();
        return;
    }

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( (!game_engine_running() || game_engine_display_team_indicators()) && !cinematic_in_progress() )
        hud_draw_players();

    if ( !game_time_get_paused()
      && (render.local_player_index == local_player_get_next(-1) || hcex_coop_local_player_index >= 0) )
        motion_sensor_tick();

    uint8_t show_hud;
    if ( hud_scripted_globals->show_hud )
    {
        if ( perspective != _director_perspective_neutral && perspective != _director_perspective_scripted
          && player->unit_index != -1 )
        {
            hud_render_weapon_interface(player);
            hud_show_action_response(player_index);
            hud_play_unit_sounds(player, hud_scripted_globals->show_hud);
            hud_render_unit_interface(player);
            hud_render_nav_points(render.local_player_index);
            hud_render_damage_indicators(render.local_player_index);
            hud_messaging_update(render.local_player_index);
            rasterizer_hud_end();
            return;
        }
        hud_show_action_response(player_index);
        show_hud = hud_scripted_globals->show_hud;
    }
    else
    {
        show_hud = 0;
    }

    hud_play_unit_sounds(player, show_hud);
    hud_messaging_update(render.local_player_index);
    rasterizer_hud_end();
}
