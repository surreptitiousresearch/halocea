/* particle_systems_render @0x8373BC98 — render every active particle system whose location is potentially
 * visible to the local viewer. Each particle_system_datum is 344 bytes; its render location lives in the
 * `location` member at offset 24, with cluster_index 0xFFFF meaning "no location / skip". */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/location.h"
#include "headers/particle_system_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t scenario_location_potentially_visible_local(const location *location);
extern void particle_system_render(int system_index);

void particle_systems_render(void)
{
    for (int index = data_next_index(particle_systems, -1); index != -1;
         index = data_next_index(particle_systems, index))
    {
        const location *systems_location =
            &DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, index)->location;
        if ((uint16_t)systems_location->cluster_index != 0xFFFF)
        {
            if (scenario_location_potentially_visible_local(systems_location))
                particle_system_render(index);
        }
    }
}
