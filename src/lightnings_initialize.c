#include <stdint.h>
#include "headers/lightning_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

data_array *lightnings_initialize(void)
{
    data_array *result = game_state_data_new("lightnings", 256, 8);
    lightning_globals.lightning_data = result;
    return result;
}
