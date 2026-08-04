/* recorded_animations_initialize @ 0x83711FB0 — 64-entry, 100-byte animation thread pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void recorded_animations_initialize(void)
{
    animation_threads = game_state_data_new("recorded animations", 64, 100);
}
