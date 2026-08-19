/* light_volumes_initialize @0x8380AA40 */
#include <stdint.h>
#include "headers/light_volume_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void light_volumes_initialize(void)
{
    light_volume_globals.light_volume_data = game_state_data_new("light volumes", 256, 8);
}
