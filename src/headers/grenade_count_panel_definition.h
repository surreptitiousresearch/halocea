#pragma once
/* grenade_count_panel_definition — the icon + numeric-count sub-panel of a grenade_hud_interface_
 * definition, plus the tick count below which the count starts flashing (hud.c). Layout from the
 * database (192 bytes). */

#include "static_hud_element_definition.h"
#include "number_hud_element_definition.h"

typedef struct grenade_count_panel_definition
{
    static_hud_element_definition background;    /* 0x00 */
    number_hud_element_definition numbers;        /* 0x68 */
    __int16                       flash_cutoff;    /* 0xBC */
    __int16                       pad;              /* 0xBE */
} grenade_count_panel_definition;                    /* 0xC0 (192 bytes) */
