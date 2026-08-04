/* display_error_damaged_media @0x83731078 — raise the "damaged media" error (_error_media_damaged). If already in an
 * error, hard-exit; otherwise queue the deferred error on the main menu (if there) or jump to the main menu
 * with the deferred error code set. Re-entrancy is guarded by in_error. */

#include "headers/widget_globals.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled a 2-field widget_globals {deferred_error[1]@0,
 * main_menu_deferred_error_code@6}; the DB-verified layout (widget_globals.h) has
 * main_menu_deferred_error_code@0x18 and deferred_error[2]@0x20. Reusing the canonical header corrects the
 * offsets (field names match). */

extern void system_exit(int code);
extern void main_goto_main_menu(void);

void display_error_damaged_media(void)
{
    if ( in_error )
    {
        system_exit(-4998);
    }
    else
    {
        in_error = 1;
        if ( we_are_at_the_main_menu )
        {
            if ( widget_globals.deferred_error[0].error_code == 0xFFFF )
            {
                widget_globals.deferred_error[0].error_code = _error_media_damaged;
                widget_globals.deferred_error[0].local_player_index = 0;
                widget_globals.deferred_error[0].modal = 0;
                widget_globals.deferred_error[0].pause_game_time = 0;
            }
        }
        else
        {
            if ( widget_globals.main_menu_deferred_error_code == 0xFFFF )
                widget_globals.main_menu_deferred_error_code = _error_media_damaged;
            main_goto_main_menu();
        }
        in_error = 0;
    }
}
