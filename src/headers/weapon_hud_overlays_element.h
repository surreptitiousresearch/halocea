#pragma once
/* weapon_hud_overlays_element — one overlay group in a weapon HUD interface. The element header fields are
 * inlined (rather than a nested weapon_hud_element_header) in the database layout. 104 bytes. */

#include <stdint.h>
#include "weapon_hud_overlay_definition.h"

typedef struct weapon_hud_overlays_element
{
    int16_t                       state_type;      /* 0x00 */
    int16_t                       runtime_flags;   /* 0x02 — bit0 = disabled */
    int16_t                       use_on_map_type; /* 0x04 — map/view-type bit selector */
    int16_t                       pad;             /* 0x06 */
    int                           unused[7];       /* 0x08 */
    weapon_hud_overlay_definition overlays;        /* 0x24 */
    int                           unused2[10];     /* 0x40 */
} weapon_hud_overlays_element;                      /* 104 bytes */
