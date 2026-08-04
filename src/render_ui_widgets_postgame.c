/* render_ui_widgets_postgame @0x83735428 — render the active UI widgets over a player's window during the
 * postgame (carnage report) sequence. For each of the two active widget slots it decides whether the
 * widget belongs to this player — widgets that render regardless of controller, widgets owned by this
 * player (or unowned), and error dialogs (shown to the matching player, or to anyone at the main menu) —
 * then renders it recursively into a clip rect sized to the window, offset for the player's split-screen
 * quadrant. Suppressed entirely while the virtual keyboard is up.
 *
 * DEVIATION: the decompiler splits the single 64-byte split-screen offset table into v17/v18/v19; the
 * disassembly shows one contiguous int16[32] run, reproduced here as split_screen_offsets (a per
 * [player-count][slot] point2d table; only {0,0} and {0,240} are reachable for the <=2-player postgame).
 * The clip rect's fused dword store decodes to {y0=0, x0=0, y1=height, x1=width}. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"


extern void *memset(void *dst, int value, unsigned int count);
extern uint8_t virtual_keyboard_active(void);
extern int16_t local_player_count(void);
extern void widget_instance_render_recursive(widget_instance *widget, rectangle2d *clip_rect, point2d offset, uint8_t focus, uint8_t use_nifty_plasma_fx);

void render_ui_widgets_postgame(int16_t local_player_index, rectangle2d *window_bounds)
{
    /* Per [player-count][slot] screen offset table (16B/player-count row, 4B/slot point2d entries). */
    __int16 split_screen_offsets[32];
    memset(split_screen_offsets, 0, sizeof(split_screen_offsets));
    split_screen_offsets[11] = 240;
    split_screen_offsets[19] = 240;
    split_screen_offsets[20] = 320;
    split_screen_offsets[21] = 240;
    split_screen_offsets[26] = 320;
    split_screen_offsets[29] = 240;
    split_screen_offsets[30] = 320;
    split_screen_offsets[31] = 240;

    if (virtual_keyboard_active())
        return;

    __int16 slot = local_player_index;
    if (local_player_index >= 0)
    {
        if (local_player_index > 1)
            slot = 1;
    }
    else
    {
        slot = 0;
    }

    for (int i = 0; i < 2; ++i)
    {
        char should_render = 0;
        widget_instance *widget = widget_globals.active_widgets[i];
        if (widget)
        {
            if (widget->render_regardless_of_controller_index == 1)
            {
                should_render = 1;
            }
            else if (widget->widget_is_error_dialog != 1)
            {
                __int16 widget_player = widget->local_player_index;
                if (!((widget_player != -1 || i) && widget_player != slot))
                    should_render = 1;
            }
            else
            {
                __int16 widget_player = widget->local_player_index;
                if (widget_player == slot || widget_player == -1 || slot == -1 || we_are_at_the_main_menu)
                    should_render = 1;
            }
        }

        if (should_render == 1)
        {
            __int16 width  = window_bounds->n[3] - window_bounds->n[1];
            __int16 height = window_bounds->n[2] - window_bounds->n[0];
            rectangle2d clip;
            clip.n[0] = 0;
            clip.n[1] = 0;
            clip.n[2] = (unsigned __int16)height;
            clip.n[3] = width;

            __int16 player_count = local_player_count();
            /* point2d table indexed by [player-count row (4 slots each)][slot], zero-based rows. */
            point2d offset = ((point2d *)split_screen_offsets)[4 * player_count + slot - 4];
            widget_instance_render_recursive(widget_globals.active_widgets[i], &clip, offset, 1u, 0);
        }
    }
}
