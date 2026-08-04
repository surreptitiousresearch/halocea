#include <stdint.h>
#include "headers/light_volume_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

data_array *light_volumes_initialize(void)
{
    data_array *result = game_state_data_new("light volumes", 256, 8);
    light_volume_globals.light_volume_data = result;
    return result;
}
