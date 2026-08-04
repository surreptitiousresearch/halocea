#pragma once
/* dump_datum — one row of the object memory dump (24 bytes), keyed by either a tag definition index or an
 * object type. Accumulates per-group counts and sizes. DB layout. */

typedef struct dump_datum
{
    int     definition_index;   /* 0x00 — -1 when the row is keyed by object_type */
    __int16 object_type;        /* 0x04 */
    __int16 maximum_size;       /* 0x06 */
    int     total_size;         /* 0x08 */
    __int16 count;              /* 0x0C */
    __int16 active_count;       /* 0x0E */
    __int16 garbage_count;      /* 0x10 */
    __int16 dead_count;         /* 0x12 */
    __int16 outside_map_count;  /* 0x14 */
    __int16 at_rest_count;      /* 0x16 */
} dump_datum;                   /* 0x18 (24 bytes) */
