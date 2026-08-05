#pragma once
#include <stdint.h>
/* vehicle_options_s — 4-byte vehicle-set options in a universal_variant: raw word plus the
 * packed per-vehicle-count bitfield view. Layout per database (union, 4 bytes). */

/* IDA anonymous bitfield struct for the __s1 view (DB: _BBD05D2D53689E3828EB6DA140E3AB26, 4 bytes) */
typedef struct _BBD05D2D53689E3828EB6DA140E3AB26
{
    uint32_t            : 10; /* bits 0-9 — padding */
    uint32_t cg_turrets : 3;  /* bits 10-12 */
    uint32_t banshees   : 3;  /* bits 13-15 */
    uint32_t rwarthogs  : 3;  /* bits 16-18 */
    uint32_t scorpions  : 3;  /* bits 19-21 */
    uint32_t ghosts     : 3;  /* bits 22-24 */
    uint32_t warthogs   : 3;  /* bits 25-27 */
    uint32_t preset     : 4;  /* bits 28-31 */
} _BBD05D2D53689E3828EB6DA140E3AB26;

typedef union vehicle_options_s
{
    unsigned int value;                              /* 0x00 — packed per-vehicle enable bits */
    struct _BBD05D2D53689E3828EB6DA140E3AB26 __s1;   /* 0x00 — bitfield view */
} vehicle_options_s;   /* 4 bytes */
