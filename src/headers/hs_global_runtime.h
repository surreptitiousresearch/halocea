#pragma once
#include <stdint.h>
/* hs_global_runtime — one entry in the runtime HaloScript global value table (hs_global_data),
 * 8 bytes per DB types_members. The 4-byte value lives at +4. */

typedef struct hs_global_runtime
{
    int16_t          identifier;    /* 0x00 */
    uint16_t pad;           /* 0x02 */
    int              value;         /* 0x04 */
} hs_global_runtime;
