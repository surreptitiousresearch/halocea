#pragma once
#include <stdint.h>
/* contrail_datum — one live contrail instance (68 bytes). Layout from the database. */

typedef struct contrail_datum
{
    int16_t          identifier;                      /* 0x00 */
    uint16_t flags;                           /* 0x02 */
    int              definition_index;                /* 0x04 */
    int              object_index;                    /* 0x08 */
    int16_t          attachment_index;                /* 0x0C */
    int16_t          density_function_index;          /* 0x0E */
    float            density;                          /* 0x10 */
    int16_t          sequence_index;                  /* 0x14 */
    int16_t          frame_index;                     /* 0x16 */
    float            texture_offset_u;                /* 0x18 */
    float            texture_offset_v;                /* 0x1C */
    float            time_until_point;                /* 0x20 */
    float            frame_time;                       /* 0x24 */
    float            expired_dt;                       /* 0x28 */
    int16_t          contrail_point_counts[4];        /* 0x2C */
    int              first_contrail_point_indices[4]; /* 0x34 */
} contrail_datum;                                     /* 68 bytes */
