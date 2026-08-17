/* light_volume_delete @0x8380AAF8 */
#include "headers/light_volume_globals.h"
#include "headers/data_array.h"

extern void datum_delete(data_array *data, int index);

void light_volume_delete(int light_volume_index)
{
    if (light_volume_index != -1)
        datum_delete(light_volume_globals.light_volume_data, light_volume_index);
}
