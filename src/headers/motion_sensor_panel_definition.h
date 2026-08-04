#pragma once
/* motion_sensor_panel_definition — the unit HUD's motion-sensor panel: a static background icon plus a
 * static foreground icon (the two rings), 240-byte DB layout. */

#include "static_hud_element_definition.h"

typedef struct motion_sensor_panel_definition
{
    static_hud_element_definition background; /* 0x00 (104) */
    static_hud_element_definition foreground; /* 0x68 (104) */
    int                           unused[8];  /* 0xD0 */
} motion_sensor_panel_definition;             /* 0xF0 = 240 bytes */
