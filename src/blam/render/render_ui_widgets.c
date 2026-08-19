/* render_ui_widgets @0x837355F8 — draw the active UI widget trees for a local player, then the global
 * fade-to-black overlay. Publishes the player index used by draw-string/icon helpers (clamped -1 -> 0),
 * suppresses all UI while Bink video inhibits it, and short-circuits to the virtual keyboard when it is up.
 * For each of the two controller slots a widget is rendered when it is eligible: render-regardless widgets
 * always; error dialogs when they target this player (or are unowned, or we have no player, or we are at
 * the main menu); ordinary widgets when unowned on the first pass or owned by this player. The clip rect
 * is the window's size at the origin. Finally, if fade_to_black is in [0,1], a full-screen 640x480 quad is
 * drawn with alpha = fade * 255 (snapping fade to 1.0 once it passes 0.95).
 *
 * DEVIATION: the decompiler fused the rectangle's two 16-bit edge stores into one dword, mis-rendering the
 * clip rect; the disasm shows {y0=0, x0=0, y1=height, x1=width} and the fade rect {0,0,480,640}. The
 * render call's point2d offset (passed by value in r5) is {0,0}. */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"


extern uint8_t bink_playback_ui_rendering_inhibited(void);
extern uint8_t virtual_keyboard_active(void);
extern void virtual_keyboard_render(void);
extern void widget_instance_render_recursive(widget_instance *widget, rectangle2d *clip_rect, point2d offset, uint8_t focus, uint8_t use_nifty_plasma_fx);
extern void draw_quad(rectangle2d *rect, unsigned int color);

void render_ui_widgets(int16_t local_player_index, const rectangle2d *window_bounds)
{
    local_player_index_for_draw_string_and_hack_in_icons =
        (local_player_index == -1) ? 0 : local_player_index;

    if (bink_playback_ui_rendering_inhibited())
        return;
    if (virtual_keyboard_active())
    {
        virtual_keyboard_render();
        return;
    }

    int clamped_player_index = local_player_index;
    if (clamped_player_index < 0)
        clamped_player_index = 0;
    else if (clamped_player_index > 1)
        clamped_player_index = 1;

    for (int controller_pass = 0; controller_pass < 2; controller_pass++)
    {
        char eligible = 0;
        widget_instance *widget = widget_globals.active_widgets[controller_pass];
        if (widget)
        {
            if (widget->render_regardless_of_controller_index == 1)
            {
                eligible = 1;
            }
            else
            {
                int16_t widget_player = widget->local_player_index;
                if (widget->widget_is_error_dialog == 1)
                {
                    if (widget_player == (int16_t)clamped_player_index
                        || widget_player == -1
                        || (int16_t)clamped_player_index == -1
                        || we_are_at_the_main_menu)
                        eligible = 1;
                }
                else if ((widget_player == -1 && controller_pass == 0)
                         || widget_player == (int16_t)clamped_player_index)
                {
                    eligible = 1;
                }
            }
        }

        if (eligible == 1)
        {
            rectangle2d clip_rect;
            clip_rect.__s1.y0 = 0;
            clip_rect.__s1.x0 = 0;
            clip_rect.__s1.y1 = window_bounds->__s1.y1 - window_bounds->__s1.y0;
            clip_rect.__s1.x1 = window_bounds->__s1.x1 - window_bounds->__s1.x0;
            point2d offset = { 0, 0 };
            widget_instance_render_recursive(widget, &clip_rect, offset, 1, 0);
        }
    }

    float fade_to_black = widget_globals.fade_to_black;
    if (fade_to_black >= 0.0f && fade_to_black <= 1.0f)
    {
        rectangle2d fade_rect;
        fade_rect.__s1.y0 = 0;
        fade_rect.__s1.x0 = 0;
        fade_rect.__s1.y1 = 480;
        fade_rect.__s1.x1 = 640;
        if (fade_to_black >= 0.94999999f)
        {
            fade_to_black = 1.0f;
            widget_globals.fade_to_black = 1.0f;
        }
        draw_quad(&fade_rect, (unsigned int)((int)(fade_to_black * 255.0f) << 24));
    }
}
