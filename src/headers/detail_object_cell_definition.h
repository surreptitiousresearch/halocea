#pragma once
#include <stdint.h>
/* detail_object_cell_definition — one detail-object grid cell entry (32 bytes, DB layout). */

typedef struct detail_object_cell_definition
{
    int16_t       cell_x;       /* 0x00 */
    int16_t       cell_y;       /* 0x02 */
    int16_t       cell_z;       /* 0x04 */
    int16_t       offset_z;     /* 0x06 */
    unsigned int  valid_layers; /* 0x08 */
    int           start_index;  /* 0x0C */
    int           count_index;  /* 0x10 */
    int           unused[3];    /* 0x14 */
} detail_object_cell_definition;
