#pragma once
/* detail_object_cell_coord — a 3D detail-object grid cell coordinate key (8 bytes, DB layout). */

typedef struct /* align(2) */ detail_object_cell_coord
{
    __int16          x;           /* 0x00 */
    __int16          y;           /* 0x02 */
    __int16          z;           /* 0x04 */
    unsigned __int8  initialized; /* 0x06 */
    unsigned __int8  pad_7;       /* 0x07 padding byte */
} detail_object_cell_coord; /* 8 bytes */
