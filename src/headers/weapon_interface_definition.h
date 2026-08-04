#pragma once
/* weapon_interface_definition — first-person / HUD interface payload embedded at offset 0x154
 * inside _weapon_definition (52 bytes). Layout DB-verified against type
 * 'weapon_interface_definition' (2026-07-13), matches ref. */

#include "tag_reference.h"

typedef struct weapon_interface_definition
{
    tag_reference   first_person_model;         /* 0x00 */
    tag_reference   first_person_animations;    /* 0x10 */
    int             unused[1];                  /* 0x20 */
    tag_reference   hud_interface;              /* 0x24 */
} weapon_interface_definition; /* 0x34 = 52 bytes */
