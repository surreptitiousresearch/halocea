#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

data_array *antennas_initialize(void)
{
    data_array *result = game_state_data_new("antenna", 12, 700);
    antenna_data = result;
    return result;
}
