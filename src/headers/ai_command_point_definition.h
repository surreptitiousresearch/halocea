#pragma once
/* ai_command_point_definition — a waypoint referenced by an AI command list (20 bytes).
 * Layout verbatim from the database. */

#include "real_point3d.h"

typedef struct ai_command_point_definition
{
    real_point3d position;      /* 0x00 */
    int          surface_index; /* 0x0C */
    unsigned int unused;        /* 0x10 */
} ai_command_point_definition;  /* 0x14 = 20 bytes */
