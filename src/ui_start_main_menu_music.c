/* ui_start_main_menu_music @0x83731130 — start the looping main-menu title music, unless it is already
 * playing or a menu fade is in progress.
 *
 * DEVIATION: the sound scale arrives as a double via the soft-float ABI; it is a float (1.0). */

#include <stdint.h>
#include "headers/widget_globals.h"

extern uint8_t main_menu_fade_active(void);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern void scripted_looping_sound_start(int definition_index, int source_object_index, float scale);

void ui_start_main_menu_music(void)
{
    if ( !widget_globals.main_menu_music_active && !main_menu_fade_active() )
    {
        /* attested: binary string is sound\music\title1\title1 — backslashes must be escaped in C */
        int sound_definition_index = tag_loaded(0x6C736E64u /* 'lsnd' */, "sound\\music\\title1\\title1");
        if ( sound_definition_index != -1 )
        {
            scripted_looping_sound_start(sound_definition_index, -1, 1.0f);
            widget_globals.main_menu_music_active = 1;
        }
    }
}
