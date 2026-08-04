#pragma once
/* auxilary_meter_definition — one entry in the unit HUD's auxilary meter block (e.g. the integrated-
 * light battery meter). 324-byte DB layout: an auxilary type index plus a full metered panel. */

#include "metered_panel_definition.h"

typedef struct auxilary_meter_definition
{
    __int16                   type;      /* 0x00 — _auxilary_meter_integrated_light etc. */
    __int16                   pad;       /* 0x02 */
    int                       unused0[4]; /* 0x04 */
    metered_panel_definition  panel;     /* 0x14 (240) */
    int                       unused1[16]; /* 0x104 */
} auxilary_meter_definition;             /* 0x144 = 324 bytes */
