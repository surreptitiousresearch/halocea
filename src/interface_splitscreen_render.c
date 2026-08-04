/* interface_splitscreen_render @0x836A6DE0 — draw the black divider bars between split-screen viewports.
 * The screen is treated as single-view (no dividers) when the engine forces single screen, during a
 * cinematic, when the UI forces fullscreen, or in co-op. With two players a single horizontal bar splits
 * top/bottom; with three or four players an additional vertical bar is drawn (full height for four
 * players, lower half for three).
 *
 * DEVIATION: the decompiler collapses the vertical bar's left/right edges into one packed dword
 * (20906464 = 0x013F0140); restored as x0=319, y1=320. */

#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/render_camera.h"
#include "headers/rectangle2d.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_force_single_screen(void);
extern uint8_t cinematic_in_progress(void);
extern uint8_t ui_widgets_force_fullscreen(int16_t *local_player_index_reference);
extern int16_t local_player_count(void);
extern void draw_quad(rectangle2d *rect, unsigned int color);

void interface_splitscreen_render(void)
{
    __int16 player_count;
    if (game_engine_force_single_screen() || cinematic_in_progress()
        || ui_widgets_force_fullscreen(nullptr) || hcex_coop_local_player_index >= 0)
        player_count = 1;
    else
        player_count = local_player_count();

    if (player_count <= 1)
        return;

    /* Horizontal divider across the vertical center, full viewport width. */
    rectangle2d divider;
    int vertical_extent = global_window_parameters.camera.viewport_bounds.n[2]
                        + global_window_parameters.camera.viewport_bounds.n[0];
    divider.n[1] = global_window_parameters.camera.viewport_bounds.n[1];
    divider.n[3] = global_window_parameters.camera.viewport_bounds.n[3];
    divider.n[0] = vertical_extent / 2 - 2;
    divider.n[2] = vertical_extent / 2 + 2;
    draw_quad(&divider, 0xFF000000);

    if (player_count > 2)
    {
        /* Vertical divider at the horizontal center. */
        divider.n[1] = 319;
        divider.n[2] = 320;
        divider.n[3] = 321;
        divider.n[0] = (player_count != 3) ? 0 : 240;
        draw_quad(&divider, 0xFF000000);
    }
}
