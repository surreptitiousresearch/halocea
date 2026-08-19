/* ai_globals_ai_active @0x836E7708 — enable/disable the AI simulation globally (ai_globals->ai_active). */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"


void ai_globals_ai_active(uint8_t enable)
{
    ai_globals->ai_active = enable;
}
