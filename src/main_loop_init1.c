/* main_loop_init1 @ 0x83688B78 — first-pass standalone loop init; calls game_initialize */

#include <stdint.h>
#include <string.h>
#include "headers/main_globals.h"

#include "headers/SYSTEMTIME.h"   /* GetLocalTime / SYSTEMTIME (avoid <windows.h> stub clash) */
extern void GetLocalTime(SYSTEMTIME *lpSystemTime);
#include "headers/blam_data_globals.h"

extern uint8_t game_in_editor(void);
extern uint32_t system_milliseconds(void);
extern void console_initialize(void);
extern int net_graph_initialize(void);
extern void hud_chat_initialize(void);
extern void debug_keys_initialize(void);
extern void game_initialize(void);
extern void sound_disable_from_commandline(uint8_t disable);


void main_loop_init1(void)
{
    SYSTEMTIME local_time;

    GetLocalTime(&local_time);
    if ( !game_in_editor() )
    {
        strncpy(main_globals.soloplayer_map_name, "levels\\test\\test", 0xFF);
        main_globals.soloplayer_map_name[255] = 0;
    }
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.halt_time_scale = 1;
    /* want_to_be_at_main_menu = !game_in_editor() */
    main_globals.want_to_be_at_main_menu = game_in_editor() == 0;
    main_globals.idle_last_activity = system_milliseconds();
    console_initialize();
    /* attested void: decompiler threaded r3 through these init helpers */
    net_graph_initialize();
    hud_chat_initialize();
    debug_keys_initialize();
    game_initialize();
    sound_disable_from_commandline(optionNoSound);
}
