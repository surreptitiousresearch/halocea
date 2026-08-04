/* machine_place @0x837BBE60 — per-map placement hook for a device machine: register its scenario device
 * information (power/position groups) and forward the scenario machine flags' low four bits (does not
 * operate automatically / one-sided / never appears locked / opened by melee attack) into the machine
 * object's device flags dword (+532). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/machine_datum.h"
#include "headers/machine_datum_flags.h"
#include "headers/scenario_machine_datum.h"
#include "headers/scenario_machine_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/scenario_device_datum.h"
extern void device_add_scenario_information(unsigned int device_index, scenario_device_datum *scenario_device);

void machine_place(int machine_index, scenario_machine_datum *scenario_machine)
{
    machine_datum *machine = (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, machine_index)->datum;

    device_add_scenario_information(machine_index, &scenario_machine->device);

    if ( (scenario_machine->flags & (1u << _scenario_machine_does_not_operate_automatically_bit)) != 0 )
        machine->machine.flags |= (1u << _machine_does_not_operate_automatically_bit);
    if ( (scenario_machine->flags & (1u << _scenario_machine_one_sided_bit)) != 0 )
        machine->machine.flags |= (1u << _machine_one_sided_bit);
    if ( (scenario_machine->flags & (1u << _scenario_machine_never_appears_locked_bit)) != 0 )
        machine->machine.flags |= (1u << _machine_never_appears_locked_bit);
    if ( (scenario_machine->flags & (1u << _scenario_machine_opened_by_melee_attack_bit)) != 0 )
        machine->machine.flags |= (1u << _machine_opened_by_melee_attack_bit);
}
