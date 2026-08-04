/* unit_is_speaking @0x837BA968 — tests the unit's current speech priority (unit.speech.current.priority,
 * a signed __int16) for being positive. `(-v & ~v) >> 31` is
 * the compiler's chosen "is v > 0" idiom (bit 31 of `-v & ~v` is set iff v is a positive 32-bit value, given
 * v is sign-extended from __int16 here). Reproduced verbatim rather than rewritten as `v > 0` to stay
 * faithful to the compiled form. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


uint8_t unit_is_speaking(uint16_t unit_index)
{
    unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int v = object->unit.speech.current.priority;
    return (unsigned int)(-v & ~v) >> 31;
}
