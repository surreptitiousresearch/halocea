#pragma once

/* scenario_cutscene_flag — an entry in the scenario's cutscene_flags tag_block. 92-byte record.
 * DB-verified layout (types_members scenario_cutscene_flag). Used by HaloScript camera/effect
 * builtins (hs_object_orient, hs_effect_new) and the nav-point HUD. */

#include "real_point3d.h"
#include "real_euler_angles2d.h"

typedef struct scenario_cutscene_flag
{
    int                   flags;        /* 0x00 */
    char                  name[32];     /* 0x04 */
    real_point3d          position;     /* 0x24 (36) */
    real_euler_angles2d   facing;       /* 0x30 (48) */
    int                   unused[9];    /* 0x38 (56) */
} scenario_cutscene_flag;              /* 0x5C = 92 bytes */
