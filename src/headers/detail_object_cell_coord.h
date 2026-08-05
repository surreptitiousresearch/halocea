#pragma once
#include <stdint.h>
/* detail_object_cell_coord — a 3D detail-object grid cell coordinate key (8 bytes, DB layout). */

typedef struct /* align(2) */ detail_object_cell_coord
{
    int16_t          x;           /* 0x00 */
    int16_t          y;           /* 0x02 */
    int16_t          z;           /* 0x04 */
    uint8_t  initialized; /* 0x06 */
    uint8_t  pad_7;       /* 0x07 padding byte */
} detail_object_cell_coord; /* 8 bytes */
