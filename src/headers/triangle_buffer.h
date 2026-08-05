#pragma once
#include <stdint.h>
/* triangle_buffer — describes one stream of structure geometry triangle indices. Layout from the database. */

typedef struct triangle_buffer
{
    int16_t type;              /* 0x00 */
    uint16_t pad;       /* 0x02 */
    int     count;              /* 0x04 */
    int     offset;             /* 0x08 */
    void   *hardware_format;    /* 0x0C */
} triangle_buffer;               /* 16 bytes */
