/* vehicle_remapper_get_type @0x83700D80 — linear search of the vehicle_remapper table for the entry whose
 * ref_index matches vehicle_ref_index; returns its row (vehicle-type) index, or -1 if not present. */

#include "headers/vehicle_remapper_s.h"

int vehicle_remapper_get_type(int vehicle_ref_index)
{
    for (int index = 0; index < 6; ++index)
    {
        if (vehicle_remapper.vehicles[index][0].ref_index == (unsigned int)vehicle_ref_index)
            return index;
    }
    return -1;
}
