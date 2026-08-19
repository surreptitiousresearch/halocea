/* unit_stop_running_blindly @0x836CD4E0 — clear the unit's "running blindly" flag (bit 0x19). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_stop_running_blindly(int unit_index)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit->unit.flags &= ~(1u << _unit_running_blindly_bit);
}
