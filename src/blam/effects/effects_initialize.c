/* effects_initialize @ 0x836E0C78 — effect + effect-location pools */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void effects_initialize(void)
{
    effect_data = game_state_data_new("effect", 256, 252);
    effect_location_data = game_state_data_new("effect location", 512, 60);
}
