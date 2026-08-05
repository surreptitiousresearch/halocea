#include <stdint.h>
#include "headers/lightning_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void lightnings_initialize(void)
{
    lightning_globals.lightning_data = game_state_data_new("lightnings", 256, 8);
}
