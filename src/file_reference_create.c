/* file_reference_create @0x83767920 — initialize an empty file reference for the given storage location. The
 * reference begins with the 'file' signature ("file" = 0x66696C65) and stores the location code at byte 6. */

#include <stdint.h>
#include "headers/file_reference.h"

extern void *memset(void *destination, int value, unsigned int size);

file_reference *file_reference_create(file_reference *reference, int16_t location)
{
    memset(reference, 0, sizeof(file_reference));
    *(int16_t *)&reference->data[6] = location;
    *(unsigned int *)reference->data = 0x66696C65u; /* 'file' */
    return reference;
}
