#pragma once
/* triangle_buffer — describes one stream of structure geometry triangle indices. Layout from the database. */

typedef struct triangle_buffer
{
    __int16 type;              /* 0x00 */
    unsigned __int16 pad;       /* 0x02 */
    int     count;              /* 0x04 */
    int     offset;             /* 0x08 */
    void   *hardware_format;    /* 0x0C */
} triangle_buffer;               /* 16 bytes */
