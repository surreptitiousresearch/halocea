/* hud_messaging_globals_update @0x836A4BA0 — per-frame HUD messaging update; in this build it only clears the
 * messaging magic number (a single byte at +0x8C5). */

#include "headers/hud_messaging_globals.h"

void hud_messaging_globals_update(void)
{
    hud_messaging_globals->magic_number = 0;
}
