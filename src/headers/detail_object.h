#pragma once
/* detail_object — a single detail-object instance record; 6-byte stride. Layout from the database. */
typedef struct detail_object
{
    unsigned char    position[3]; /* 0x00 — 8-bit fixed-point cell-local offset, one per axis */
    unsigned char    data;        /* 0x03 — packed: high nibble = type-definition selector source,
                                    *        low nibble = animation-frame sub-selector */
    unsigned __int16 color;       /* 0x04 */
} detail_object; /* 6 bytes */
