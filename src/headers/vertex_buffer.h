#pragma once
#include <stdint.h>
/* vertex_buffer — describes one stream of structure geometry vertices. `type` selects the vertex
 * format (1 = compressed, 0/12 = uncompressed); `count` is the vertex count and `base_address` points
 * at the packed vertex data. */

typedef struct vertex_buffer
{
    int16_t  type;            /* 0x00 */
    unsigned short pad;       /* 0x02 */
    int      count;           /* 0x04 */
    int      offset;          /* 0x08 */
    void    *base_address;    /* 0x0C */
    void    *hardware_format; /* 0x10 */
} vertex_buffer;              /* 20 bytes */
