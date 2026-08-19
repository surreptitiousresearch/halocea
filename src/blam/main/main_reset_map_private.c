/* main_reset_map_private @ 0x8368A6C8 — hard reset the current map: tear down old map,
 * flush input, re-init for the new map, recreate local players and restart the clock. */
#include <stdint.h>
#include "headers/main_globals.h"

extern uint8_t game_time_get_paused(void);
extern int game_time_get(void);
extern void hcex_output_dbg(const char *, ...);
extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);
extern void game_dispose_from_old_map(void);
extern void input_flush(void);
extern void input_abstraction_flush(void);
extern void game_initialize_for_new_map(void);
extern void create_local_players(void);
extern void game_time_start(void);
extern void game_initial_pulse(void);
extern void ui_widgets_disable_pause_game(int duration_ticks);

void main_reset_map_private(void)
{
    if ( game_time_get_paused() )
        return;
    hcex_output_dbg("main_reset_map_private tick %d\n", game_time_get());
    /* Deviation: the decompiler threaded these void/return-discarded calls through a
     * dead r3 local; the disasm sets no args between them. Unchained. */
    scenario_switch_structure_bsp(0);
    game_dispose_from_old_map();
    input_flush();
    input_abstraction_flush();
    game_initialize_for_new_map();
    create_local_players();
    game_time_start();
    game_initial_pulse();
    ui_widgets_disable_pause_game(30);
    main_globals.reset_map = 0;
}
