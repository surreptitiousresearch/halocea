/* device_set_never_appears_locked @0x837B4EB8 — set/clear the "never appears locked" flag (machine_datum
 * machine.flags @+0x214 bit 0x4), which suppresses the locked-door HUD indicator. */

#include <stdint.h>
#include "headers/machine_datum.h"
#include "headers/machine_datum_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void device_set_never_appears_locked(int device_index, uint8_t never_appears_locked)
{
    if ( device_index == -1 )
        return;
    machine_datum *device = object_try_and_get_and_verify_type(device_index, object_mask_machine);
    if ( device )
    {
        unsigned int flags = device->machine.flags;
        device->machine.flags = never_appears_locked ? (flags | (1u << _machine_never_appears_locked_bit)) : (flags & ~(1u << _machine_never_appears_locked_bit));
    }
}
