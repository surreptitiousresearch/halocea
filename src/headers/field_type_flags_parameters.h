#pragma once
/* _field_type_flags_parameters — the `parameters` blob for a "flags" message-delta field (36 bytes,
 * DB layout, confirmed by headers_ref `_field_type_flags_parameters` sizeof=0x24): how many flag bits
 * the field carries, plus a per-bit presence table (indices 0..count-1 are the live entries). */

#include <stdint.h>

typedef struct _field_type_flags_parameters
{
    const int      count;              /* 0x00 */
    const uint8_t  flags_present[32];  /* 0x04 */
} _field_type_flags_parameters; /* 36 bytes */
