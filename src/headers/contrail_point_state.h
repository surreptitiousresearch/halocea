#pragma once
/* contrail_point_state — one element of contrail_definition.states (104 bytes; DB-verified,
   matches headers_ref). */

#include "tag_reference.h"
#include "real_argb_color.h"

typedef struct contrail_point_state
{
    float           duration_lower_bound;            /* 0x00 */
    float           duration_upper_bound;            /* 0x04 */
    float           transition_duration_lower_bound; /* 0x08 */
    float           transition_duration_upper_bound; /* 0x0C */
    tag_reference   physics;                         /* 0x10 */
    int             unused0[8];                      /* 0x20 */
    float           width;                           /* 0x40 */
    real_argb_color color_lower_bound;               /* 0x44 */
    real_argb_color color_upper_bound;               /* 0x54 */
    unsigned int    scale_flags;                     /* 0x64 */
} contrail_point_state;                              /* 104 bytes */
