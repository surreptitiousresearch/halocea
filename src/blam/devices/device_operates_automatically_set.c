/* device_operates_automatically_set @0x837B5070 — set/clear the "does not operate automatically" flag
 * (machine_datum machine.flags @+0x214 bit 0x1). The stored bit is inverted: it is SET when automatic operation is
 * disabled, so passing operates_automatically=true clears it. */

#include <stdint.h>
#include "headers/machine_datum.h"
#include "headers/machine_datum_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void device_operates_automatically_set(int device_index, uint8_t operates_automatically)
{
    machine_datum *device = object_try_and_get_and_verify_type(device_index, object_mask_machine);
    if ( device )
    {
        unsigned int flags = device->machine.flags;
        device->machine.flags = operates_automatically ? (flags & ~(1u << _machine_does_not_operate_automatically_bit)) : (flags | (1u << _machine_does_not_operate_automatically_bit));
    }
}
