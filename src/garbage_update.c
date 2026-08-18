/* garbage_update @0x837EA108 — decrement the object's despawn timer (+556); when it reaches (or wraps past)
 * zero, delete the object. Returns 1 while the timer is still nonzero (object survives), 0 when it hit zero
 * and the object was deleted. Bit-trick faithfully reproduced from disasm_range(0x837EA108,0x837EA170)
 * (`-timer & ~timer`; the sign bit is set iff timer != 0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/garbage_datum.h"
#include "headers/blam_data_globals.h"

extern void object_delete(int object_index);

uint8_t garbage_update(int garbage_index)
{
    garbage_datum *garbage = (garbage_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, garbage_index)->datum;
    int16_t timer = garbage->garbage.destroy_timer - 1;

    garbage->garbage.destroy_timer = timer;

    int zero_test = -timer & ~timer;
    unsigned int is_zero = (unsigned int)zero_test >> 31;

    if ( zero_test >= 0 )
        object_delete(garbage_index);
    return is_zero;
}
