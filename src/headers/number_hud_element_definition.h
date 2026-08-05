#pragma once
/* number_hud_element_definition — a numeric HUD element (digit count + color + placement), hud.c.
 * Layout from the database (84 bytes). */

#include <stdint.h>
#include "hud_placement_definition.h"
#include "hud_color_definition.h"

typedef struct number_hud_element_definition
{
    hud_placement_definition placement;     /* 0x00 */
    hud_color_definition     colors;        /* 0x24 */
    char            digits;                 /* 0x44 */
    uint8_t number_flags;           /* 0x45 */
    char            fractional_digits;      /* 0x46 */
    uint8_t pad;                    /* 0x47 */
    int             unused1[3];             /* 0x48 */
} number_hud_element_definition;            /* 0x54 */
