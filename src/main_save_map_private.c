/* main_save_map_private @ 0x8368A2F0 — decide whether it is safe to autosave this tick.
 * "Unsafe" saves go immediately; safe saves wait for game_safe_to_save() to hold for 3
 * consecutive checks, giving up after 240 ticks if save_map_timeout is set. */
#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern void console_printf(uint8_t clear, const char *format, ...);
extern uint8_t game_safe_to_save(void);
extern void hud_autosave(uint8_t begin);
extern uint8_t game_time_get_paused(void);

void main_save_map_private(void)
{
    char do_save;

    if ( game_time_get_paused() )
        return;

    do_save = 0;
    if ( !main_globals.save_map_safely )
    {
        if ( debug_game_save )
            console_printf(0, "unsafe save");
        do_save = 1;
    }
    else
    {
        char within_window = main_globals.ticks_unable_to_save++ < 240;
        if ( !within_window && main_globals.save_map_timeout )
        {
            if ( debug_game_save )
                console_printf(0, "gave up trying to save");
            main_globals.save_map = 0;
        }
        else if ( main_globals.ticks_until_next_save_check-- <= 0 )
        {
            if ( game_safe_to_save() )
            {
                if ( main_globals.safe_intervals++ >= 3 )
                {
                    do_save = 1;
                    main_globals.ticks_until_next_save_check = 10;
                }
            }
            else
            {
                main_globals.safe_intervals = 0;
            }
            main_globals.ticks_until_next_save_check = 10;
        }
    }

    if ( do_save )
    {
        hud_autosave(1);
        main_globals.saving_map = 1;
        main_globals.save_map = 0;
    }
}
