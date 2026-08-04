/* main_revert_map_private @0x8368A0D8 — perform a queued checkpoint revert: roll back game state, briefly
 * disable the pause UI, and clear the revert request. */

#include "headers/main_globals.h"

extern int game_time_get(void);
extern void hcex_output_dbg(const char *format, ...);
extern void game_state_revert(void);
extern void ui_widgets_disable_pause_game(int duration_ticks);

void main_revert_map_private(void)
{
    hcex_output_dbg("main_revert_map_private tick %d\n", game_time_get());
    game_state_revert();
    ui_widgets_disable_pause_game(30);
    main_globals.revert_map = 0;
}
