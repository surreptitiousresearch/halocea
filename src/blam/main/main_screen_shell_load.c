/* main_screen_shell_load @0x837366F8 — start the main-menu music if it is not already playing.
 * Attested void: the compiled code tail-branches (b) into ui_start_main_menu_music, and the
 * already-active path returns via bnelr with r3 untouched; the sole caller ignores r3. */

#include "headers/widget_globals.h"

extern void ui_start_main_menu_music(void);

void main_screen_shell_load(void)
{
    if ( !widget_globals.main_menu_music_active )
        ui_start_main_menu_music();
}
