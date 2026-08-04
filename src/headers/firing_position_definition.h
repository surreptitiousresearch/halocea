#pragma once
/* firing_position_definition — static AI firing-position slot from the tag/BSP. DB layout. */

#include "real_point3d.h"

typedef struct firing_position_definition
{
    real_point3d position;         /* 0x00 */
    __int16      group_index;      /* 0x0C */
    __int16      cluster_index;    /* 0x0E */
    int          pad;              /* 0x10 */
    int          surface_index;    /* 0x14 */
} firing_position_definition;      /* 24 bytes */
