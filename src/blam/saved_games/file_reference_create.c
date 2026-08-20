/* file_reference_create @0x83767920 — initialize an empty file reference for the given storage location. The
 * reference begins with the 'filo' signature (0x66696C6F) and stores the location code at byte 6. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"


file_reference *file_reference_create(file_reference *reference, int16_t location)
{
    memset(reference, 0, sizeof(file_reference));
    reference->info.location = location;
    /* DEVIATION: signature was 0x66696C65 ('file'); the binary composes 0x66696C6F ('filo') via
     * lis r11,0x6669 @0x83767948 + ori r11,r11,0x6C6F @0x83767954, stored by stw r11,0(r31) @0x83767958. */
    reference->info.signature = 0x66696C6Fu; /* 'filo' */
    return reference;
}
