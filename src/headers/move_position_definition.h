#pragma once
/* move_position_definition — a single scripted move-position for an actor's alert behavior
 * (80 bytes, types_members-confirmed layout). Embedded in alert_state_data. */

#include <stdint.h>
#include "real_point3d.h"

typedef struct move_position_definition
{
    real_point3d     position;                    /* 0x00 */
    float            facing;                       /* 0x0C */
    float            weight;                       /* 0x10 */
    float            time_lower_bound;             /* 0x14 */
    float            time_upper_bound;             /* 0x18 */
    int16_t          animation_reference_index;    /* 0x1C */
    char             sequence_id;                  /* 0x1E */
    uint8_t  pad2;                         /* 0x1F */
    int              unused[2];                    /* 0x20 */
    int16_t          cluster_index;                /* 0x28 */
    uint16_t pad;                          /* 0x2A */
    int              unused2[8];                   /* 0x2C */
    int              surface_index;                /* 0x4C */
} move_position_definition; /* 80 bytes */
