#pragma once
/* contrail_datum — one live contrail instance (68 bytes). Layout from the database. */

typedef struct contrail_datum
{
    __int16          identifier;                      /* 0x00 */
    unsigned __int16 flags;                           /* 0x02 */
    int              definition_index;                /* 0x04 */
    int              object_index;                    /* 0x08 */
    __int16          attachment_index;                /* 0x0C */
    __int16          density_function_index;          /* 0x0E */
    float            density;                          /* 0x10 */
    __int16          sequence_index;                  /* 0x14 */
    __int16          frame_index;                     /* 0x16 */
    float            texture_offset_u;                /* 0x18 */
    float            texture_offset_v;                /* 0x1C */
    float            time_until_point;                /* 0x20 */
    float            frame_time;                       /* 0x24 */
    float            expired_dt;                       /* 0x28 */
    __int16          contrail_point_counts[4];        /* 0x2C */
    int              first_contrail_point_indices[4]; /* 0x34 */
} contrail_datum;                                     /* 68 bytes */
