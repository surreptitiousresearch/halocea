#pragma once

#include "path_destination.h"
#include "path_step.h"

/* path_result — Blam pathfinding output buffer (92 bytes). Layout confirmed via types_members. */
typedef struct path_result
{
    unsigned __int8   valid;              /* 0x00 */
    unsigned char     _pad1[3];           /* 0x01 */
    path_destination  endpoint;           /* 0x04 */
    unsigned __int8   steps_finish_path;  /* 0x18 */
    char              step_count;         /* 0x19 */
    char              step_index;         /* 0x1A */
    unsigned char     _pad2;              /* 0x1B */
    path_step         steps[4];           /* 0x1C */
} path_result; /* 0x5C */
