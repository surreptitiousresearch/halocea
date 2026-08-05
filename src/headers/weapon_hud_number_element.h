#pragma once
/* weapon_hud_number_element — one numeric readout element in a weapon HUD interface. Layout from the
 * database (160 bytes). */

#include <stdint.h>
#include "weapon_hud_element_header.h"
#include "number_hud_element_definition.h"

typedef struct weapon_hud_number_element
{
    weapon_hud_element_header     header;         /* 0x00 */
    number_hud_element_definition number_element; /* 0x24 */
    uint16_t              weapon_flags;   /* 0x78 — bit0 = divide by magazine size */
    int16_t                       pad;            /* 0x7A */
    int                           unused[9];      /* 0x7C */
} weapon_hud_number_element;                       /* 160 bytes */
