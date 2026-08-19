/* hud_messaging_initialize_for_new_map @0x836A3FA0 — point the active HUD messaging definition at the current
 * HUD globals' messaging block and clear the HUD messaging runtime state. */

#include <string.h>
#include "headers/hud_messaging_globals.h"

void hud_messaging_initialize_for_new_map(void)
{
    hud_msg_def = &hud_globals->messaging;
    /* tail-call to memset in the binary; return value (dst) is discarded — 0/1 callers consume r3 => void */
    memset(hud_messaging_globals, 0, sizeof(hud_messaging_globals_definition));
}
