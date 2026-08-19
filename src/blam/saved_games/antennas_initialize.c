/* antennas_initialize @0x8380D2C8 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void antennas_initialize(void)
{
    antenna_data = game_state_data_new("antenna", 12, 700);
}
