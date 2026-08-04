#pragma once
/* weapon_hud_meter_element — one meter (bar) element in a weapon HUD interface. Layout from the
 * database (180 bytes). */

#include "weapon_hud_element_header.h"
#include "meter_hud_element_definition.h"

typedef struct weapon_hud_meter_element
{
    weapon_hud_element_header    header;        /* 0x00 */
    meter_hud_element_definition meter_element; /* 0x24 */
    int                          unused[10];    /* 0x8C */
} weapon_hud_meter_element;                      /* 180 bytes */
