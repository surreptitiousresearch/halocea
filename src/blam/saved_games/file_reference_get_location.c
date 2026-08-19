/* file_reference_get_location @0x83767A50 — accessor for a file_reference's location id
 * (_file_reference_application_relative / _cd_relative / _absolute).
 * DEVIATION: the decompiler's raw `*(int16_t *)(reference + 6)` is file_reference_info.location
 * (DB types_members: __int16 at +6); disasm 0x83767A50 is a single `lhz r3, 6(r3)`. */

#include <stdint.h>
#include "headers/file_reference.h"

int16_t file_reference_get_location(const file_reference *reference)
{
    return reference->info.location;
}
