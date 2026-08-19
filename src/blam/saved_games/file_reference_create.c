/* file_reference_create @0x83767920 — initialize an empty file reference for the given storage location. The
 * reference begins with the 'file' signature ("file" = 0x66696C65) and stores the location code at byte 6. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"


file_reference *file_reference_create(file_reference *reference, int16_t location)
{
    memset(reference, 0, sizeof(file_reference));
    reference->info.location = location;
    reference->info.signature = 0x66696C65u; /* 'file' */
    return reference;
}
