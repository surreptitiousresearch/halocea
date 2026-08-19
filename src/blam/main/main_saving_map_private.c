/* main_saving_map_private @0x8368A290 — commit a queued map save (unless a revert is also queued): save
 * game state and refresh the autosave HUD indicator. */

#include <stdint.h>
#include "headers/main_globals.h"

extern int game_time_get(void);
extern void hcex_output_dbg(const char *format, ...);
extern void game_state_save(uint8_t do_autosave);
extern void hud_autosave(uint8_t begin);

void main_saving_map_private(void)
{
    main_globals.saving_map = 0;
    if ( !main_globals.revert_map )
    {
        hcex_output_dbg("main_saving_map_private tick %d\n", game_time_get());
        game_state_save(1u);
        hud_autosave(0);
    }
}
