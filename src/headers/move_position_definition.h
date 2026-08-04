#pragma once
/* move_position_definition — a single scripted move-position for an actor's alert behavior
 * (80 bytes, types_members-confirmed layout). Embedded in alert_state_data. */

#include "real_point3d.h"

typedef struct move_position_definition
{
    real_point3d     position;                    /* 0x00 */
    float            facing;                       /* 0x0C */
    float            weight;                       /* 0x10 */
    float            time_lower_bound;             /* 0x14 */
    float            time_upper_bound;             /* 0x18 */
    __int16          animation_reference_index;    /* 0x1C */
    char             sequence_id;                  /* 0x1E */
    unsigned __int8  pad2;                         /* 0x1F */
    int              unused[2];                    /* 0x20 */
    __int16          cluster_index;                /* 0x28 */
    unsigned __int16 pad;                          /* 0x2A */
    int              unused2[8];                   /* 0x2C */
    int              surface_index;                /* 0x4C */
} move_position_definition; /* 80 bytes */
