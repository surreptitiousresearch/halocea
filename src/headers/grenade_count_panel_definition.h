#pragma once
/* grenade_count_panel_definition — the icon + numeric-count sub-panel of a grenade_hud_interface_
 * definition, plus the tick count below which the count starts flashing (hud.c). Layout from the
 * database (192 bytes). */

#include <stdint.h>
#include "static_hud_element_definition.h"
#include "number_hud_element_definition.h"

typedef struct grenade_count_panel_definition
{
    static_hud_element_definition background;    /* 0x00 */
    number_hud_element_definition numbers;        /* 0x68 */
    int16_t                       flash_cutoff;    /* 0xBC */
    int16_t                       pad;              /* 0xBE */
} grenade_count_panel_definition;                    /* 0xC0 (192 bytes) */
