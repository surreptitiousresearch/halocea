#pragma once
/* file_reference_info — the named view of a file_reference's leading 0x108 bytes: the 'filo' signature,
 * the flags word (bit 0 = the reference names a file rather than a directory), the file-location id
 * (_file_reference_application_relative / _cd_relative / _absolute) and the location-relative path.
 * DB type file_reference_info (types_members; sizeof 0x108). Corroborated by the disassembly:
 * file_reference_create @0x83767920 stamps 0x66696C6F ("filo") at +0 and the location at +6, and
 * file_reference_get_name @0x83767A58 passes +8 as `path` and +4 bit 0 as `has_filename`. */

#include <stdint.h>

typedef struct file_reference_info
{
    uint32_t signature;                /* 0x000 */
    uint16_t flags;                    /* 0x004 */
    int16_t  location;                 /* 0x006 */
    char     path[256];                /* 0x008 */
} file_reference_info;                  /* 0x108 = 264 bytes */
