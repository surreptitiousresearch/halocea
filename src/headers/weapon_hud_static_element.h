#pragma once
/* weapon_hud_static_element — one static bitmap element in a weapon HUD interface. Layout from the
 * database (180 bytes). */

#include "weapon_hud_element_header.h"
#include "static_hud_element_definition.h"

typedef struct weapon_hud_static_element
{
    weapon_hud_element_header     header;         /* 0x00 */
    static_hud_element_definition static_element; /* 0x24 */
    int                           unused[10];     /* 0x8C */
} weapon_hud_static_element;                       /* 180 bytes */
