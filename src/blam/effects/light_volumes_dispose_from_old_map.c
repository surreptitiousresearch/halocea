/* light_volumes_dispose_from_old_map @0x8380AA98 */
#include "headers/light_volume_globals.h"
#include "headers/data_array.h"

void light_volumes_dispose_from_old_map(void)
{
    if (light_volume_globals.light_volume_data)
        data_make_invalid(light_volume_globals.light_volume_data);
}
