#pragma once
/* game_globals_first_person_interface — one element of game_globals' first_person_interface block:
 * hands model + first-person HUD references. Layout DB-verified against type
 * 'game_globals_first_person_interface' (192 bytes, 2026-07-14). */

#include "tag_reference.h"
#include "point2d.h"

typedef struct game_globals_first_person_interface
{
    tag_reference hands;                      /* 0x00 */
    tag_reference hud_base;                   /* 0x10 */
    tag_reference hud_shield_meter;           /* 0x20 */
    point2d       hud_shield_meter_origin;    /* 0x30 */
    tag_reference hud_body_meter;             /* 0x34 */
    point2d       hud_body_meter_origin;      /* 0x44 */
    tag_reference night_vision_off_on_effect; /* 0x48 */
    tag_reference night_vision_on_off_effect; /* 0x58 */
    unsigned int  unused[22];                 /* 0x68 */
} game_globals_first_person_interface;        /* 192 bytes */
