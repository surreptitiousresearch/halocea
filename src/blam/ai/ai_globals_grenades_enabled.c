/* ai_globals_grenades_enabled @0x836E7728 — enable/disable AI grenade use (ai_globals->grenades_enabled). */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"


void ai_globals_grenades_enabled(uint8_t enable)
{
    ai_globals->grenades_enabled = enable;
}
