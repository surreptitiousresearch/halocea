#pragma once
/* unit_hud_interface_definition — the unit HUD interface tag ('unhi'). Aggregates every panel drawn by
 * hud_render_unit_interface: the background icon, the shield/health metered panels, the motion-sensor
 * panel and blip placement, the auxilary overlay panel, and tag_blocks of warning sounds and auxilary
 * meters. 1388-byte DB layout. */

#include "hud_absolute_placement_definition.h"
#include "static_hud_element_definition.h"
#include "metered_panel_definition.h"
#include "motion_sensor_panel_definition.h"
#include "hud_placement_definition.h"
#include "auxilary_panel_definition.h"
#include "tag_block.h"

typedef struct unit_hud_interface_definition
{
    hud_absolute_placement_definition absolute_placement; /* 0x000 (36)  */
    static_hud_element_definition     background;         /* 0x024 (104) */
    metered_panel_definition          shield_meter;       /* 0x08C (240) */
    metered_panel_definition          health_meter;       /* 0x17C (240) */
    motion_sensor_panel_definition    motion_sensor;      /* 0x26C (240) */
    hud_placement_definition          blip_placement;     /* 0x35C (36)  */
    auxilary_panel_definition         auxilary_panel;     /* 0x380 (64)  */
    tag_block                         warning_sounds;     /* 0x3C0 (12)  */
    tag_block                         auxilary_meters;    /* 0x3CC — auxilary_meter_definition[] */
    int                               unused1[89];        /* 0x3D8 */
    int                               unused2[12];        /* 0x53C */
} unit_hud_interface_definition;                          /* 0x56C = 1388 bytes */
