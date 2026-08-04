/* file_reference_get_location @0x83767A50 — accessor for file_reference's drive/path id word at +6.
 * file_reference is modeled as an opaque byte block (see file_reference.h); no field name exists for
 * this offset yet. */

#include <stdint.h>
#include "headers/file_reference.h"

int16_t file_reference_get_location(const file_reference *reference)
{
    return *(const __int16 *)(reference->data + 6);
}
