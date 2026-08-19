/* ai_flush_spatial_effects @ 0x836E98C0 — reset the AI spatial-effect (noise /
 * stimulus) ring buffer: clear both head/tail indices and zero the entire array. */

#include <string.h>
#include "headers/ai_globals.h"

void * ai_flush_spatial_effects(void)
{
    ai_globals->spatial_effects_last_index = 0;
    ai_globals->spatial_effects_first_index = 0;
    return memset(ai_globals->spatial_effects, 0, sizeof(ai_globals->spatial_effects));
}
