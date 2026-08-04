/* unit_driven_by_ai @0x836CA868 — true if the unit (or its vehicle driver, if any) is currently controlled by
 * an actor rather than a player. Simplified the decompiler's branchless (x+1-x-(x==-1)) arithmetic to the
 * equivalent (x != -1) it computes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


uint8_t unit_driven_by_ai(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int driver_object_index = unit->unit.driver_object_index;
    if ( driver_object_index != -1 )
        unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, driver_object_index)->datum;
    return unit->unit.actor_index != -1;
}
