#pragma once
/* weapon_hud_element_header — common leading record shared by every weapon-HUD element (static/meter/number/
 * overlay). Selects which weapon HUD state drives the element and on which map/view types it renders.
 * Layout from the database (36 bytes). */

typedef struct weapon_hud_element_header
{
    __int16 state_type;       /* 0x00 — index into the per-weapon state/value arrays */
    __int16 runtime_flags;    /* 0x02 — bit0 = disabled */
    __int16 use_on_map_type;  /* 0x04 — map/view-type bit selector */
    __int16 pad;              /* 0x06 */
    int     unused[7];        /* 0x08 */
} weapon_hud_element_header;  /* 36 bytes */
