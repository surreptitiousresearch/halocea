#pragma once
/* hud_multiplayer_parameters_definition — HUD globals sub-block for multiplayer-only HUD
 * parameters (DB types_members-confirmed, 260 bytes). Only `hud_scale` has a known consumer
 * so far; the remainder is kept opaque. */

typedef struct hud_multiplayer_parameters_definition
{
    float hud_scale;  /* 0x00 */
    int   unused[64]; /* 0x04 */
} hud_multiplayer_parameters_definition; /* 260 bytes */
