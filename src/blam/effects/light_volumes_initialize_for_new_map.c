/* light_volumes_initialize_for_new_map @0x8380AA80 */
#include "headers/light_volume_globals.h"
#include "headers/data_array.h"

void light_volumes_initialize_for_new_map(void)
{
    if (light_volume_globals.light_volume_data)
        data_make_valid(light_volume_globals.light_volume_data);
}
