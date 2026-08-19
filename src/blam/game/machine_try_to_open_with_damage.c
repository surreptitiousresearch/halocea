/* machine_try_to_open_with_damage @0x837BC430 — when a machine (device object) takes damage, drive it
 * fully open if its device state allows it. machine.flags bit 3 (_machine_opened_by_melee_attack)
 * gates the action. device_set_actual_position(..., 1.0) snaps the device to its open extreme. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/machine_datum.h"
#include "headers/machine_datum_flags.h"
#include "headers/blam_data_globals.h"

extern void device_set_actual_position(int device_index, float value);

void machine_try_to_open_with_damage(int machine_index)
{
    machine_datum *machine = (machine_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, machine_index)->datum;

    if ( (machine->machine.flags & (1u << _machine_opened_by_melee_attack_bit)) != 0 )
        device_set_actual_position(machine_index, 1.0f);
}
