/* scripted_hud_restart_flashing @0x836A40A8 — restart the HUD flash animation from the current game time, if
 * flashing is currently enabled. */

#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void scripted_hud_restart_flashing(void)
{
    if ( hud_messaging_globals->use_flash )
        hud_messaging_globals->flash_start_time = game_time_get();
}
