#pragma once
/* hud_defaults_definition — the `defaults` block of hud_globals (offset 0x2C0). Holds the fallback weapon
 * HUD tag and the motion-sensor / title-bounds defaults. Layout from the database (0x50 bytes). */

#include "tag_reference.h"
#include "rectangle2d.h"

typedef struct hud_defaults_definition
{
    tag_reference default_weapon_hud;                  /* 0x00 */
    float         motion_sensor_range;                 /* 0x10 */
    float         motion_sensor_velocity_sensitivity;  /* 0x14 */
    float         motion_sensor_scale;                 /* 0x18 */
    rectangle2d   default_title_bounds;                /* 0x1C */
    int           unused[11];                          /* 0x24 */
} hud_defaults_definition;                             /* 0x50 */
