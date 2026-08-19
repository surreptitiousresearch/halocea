/* scripted_hud_set_flashing_state @0x836A4038 — turn HUD message flashing on or off, recording the game time
 * flashing began (so the flash animation phase is consistent) when first enabled. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void scripted_hud_set_flashing_state(uint8_t flash)
{
    if ( flash && !hud_messaging_globals->use_flash )
        hud_messaging_globals->flash_start_time = game_time_get();
    hud_messaging_globals->use_flash = flash;
}
