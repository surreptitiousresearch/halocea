#pragma once
#include <stdint.h>
/* dump_datum — one row of the object memory dump (24 bytes), keyed by either a tag definition index or an
 * object type. Accumulates per-group counts and sizes. DB layout. */

typedef struct dump_datum
{
    int     definition_index;   /* 0x00 — -1 when the row is keyed by object_type */
    int16_t object_type;        /* 0x04 */
    int16_t maximum_size;       /* 0x06 */
    int     total_size;         /* 0x08 */
    int16_t count;              /* 0x0C */
    int16_t active_count;       /* 0x0E */
    int16_t garbage_count;      /* 0x10 */
    int16_t dead_count;         /* 0x12 */
    int16_t outside_map_count;  /* 0x14 */
    int16_t at_rest_count;      /* 0x16 */
} dump_datum;                   /* 0x18 (24 bytes) */
