/* main_screen_shell_begin_fade @0x83733160 — begin fading out the main menu: stop the title music if it is
 * playing, then for each local player's active widget (that is not an error dialog) arm an auto-close fade
 * of the requested duration and dispose that player's widget stack.
 *
 * Deviation: the decompiler walks a _widget_globals* by 4 bytes per iteration, reading active_widgets[0] /
 * widget_stack[0] off the moving pointer and stopping at &widget_globals.widget_stack; this is iteration
 * over the two per-player slots (active_widgets[0..1] / widget_stack[0..1]). Re-expressed as an indexed loop.
 * 0x6C736E64 is the 'lsnd' (sound) group tag. */

#include <stdint.h>
#include "headers/widget_globals.h"

#include "headers/widget_stack_node.h"
extern int tag_loaded(uint32_t group_tag, const char *name);
extern void scripted_looping_sound_stop(int definition_index);
extern void dispose_widget_stack(widget_stack_node **top);

void main_screen_shell_begin_fade(unsigned int fade_duration_milliseconds)
{
    if ( widget_globals.main_menu_music_active == 1 )
    {
        int title_music = tag_loaded(0x6C736E64u /* 'lsnd' */, "sound\\music\\title1\\title1");  /* binary string @0x8211CE8C is literal backslashes; \m is an invalid C escape and \t was a TAB */
        if ( title_music != -1 )
            scripted_looping_sound_stop(title_music);
        widget_globals.main_menu_music_active = 0;
    }

    for ( int slot = 0; slot < 2; ++slot )
    {
        widget_instance *widget = widget_globals.active_widgets[slot];
        if ( widget && !widget->widget_is_error_dialog )
        {
            widget->auto_close_fade_time = fade_duration_milliseconds;
            widget->milliseconds_to_auto_close =
                widget_globals.current_system_milliseconds - widget->creation_time + 100;
            if ( widget_globals.widget_stack[slot] )
                dispose_widget_stack(&widget_globals.widget_stack[slot]);
        }
    }
}
