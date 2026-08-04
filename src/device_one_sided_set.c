/* device_one_sided_set @0x837B5018 — set/clear the "one-sided" flag (machine_datum machine.flags @+0x214 bit 0x2),
 * which restricts which side can operate the device. */

#include <stdint.h>
#include "headers/machine_datum.h"
#include "headers/machine_datum_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void device_one_sided_set(int device_index, uint8_t one_sided)
{
    machine_datum *device = object_try_and_get_and_verify_type(device_index, object_mask_machine);
    if ( device )
    {
        unsigned int flags = device->machine.flags;
        device->machine.flags = one_sided ? (flags | (1u << _machine_one_sided_bit)) : (flags & ~(1u << _machine_one_sided_bit));
    }
}
