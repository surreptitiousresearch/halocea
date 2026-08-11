/* ai_globals_dialogue_triggers_enabled @0x836E7718 — enable/disable scripted dialogue triggers (ai_globals->dialogue_triggers_enabled). */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"


void ai_globals_dialogue_triggers_enabled(uint8_t enable)
{
    ai_globals->dialogue_triggers_enabled = enable;
}
