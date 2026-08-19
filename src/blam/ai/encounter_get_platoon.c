/* encounter_get_platoon @0x837090A8 */
#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/platoon_datum.h"
#include "headers/blam_data_globals.h"


platoon_datum * encounter_get_platoon(encounter_datum *encounter, int16_t platoon_index)
{
    return &platoon_array[(short)(encounter->platoon_base + platoon_index)];
}
