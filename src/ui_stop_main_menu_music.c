/* ui_stop_main_menu_music @0x837311D0 — stop the looping main-menu title music if it is playing, and
 * clear the active flag. */

#include <stdint.h>
#include "headers/widget_globals.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern void scripted_looping_sound_stop(int definition_index);

void ui_stop_main_menu_music(void)
{
    if ( widget_globals.main_menu_music_active == 1 )
    {
        /* attested: binary string is sound\music\title1\title1 — backslashes must be escaped in C */
        int sound_definition_index = tag_loaded(0x6C736E64u /* 'lsnd' */, "sound\\music\\title1\\title1");
        if ( sound_definition_index != -1 )
            scripted_looping_sound_stop(sound_definition_index);
        widget_globals.main_menu_music_active = 0;
    }
}
