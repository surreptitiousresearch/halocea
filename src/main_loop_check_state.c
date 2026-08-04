/* main_loop_check_state @0x83688D20 — service a pending level change: load the requested solo map, or fall
 * back to the UI (main menu) level when that is wanted. */

#include "headers/main_globals.h"

extern void hcex_load_level(const char *level_name);

void main_loop_check_state(void)
{
    if ( main_globals.rename_map )
        hcex_load_level(main_globals.soloplayer_map_name);
    else if ( main_globals.want_to_be_at_main_menu )
        /* backslashes escaped 2026-07-30 (DB string 0x8210F1D8 = levels\ui\ui; "\u" was ill-formed) */
        hcex_load_level("levels\\ui\\ui");
}
