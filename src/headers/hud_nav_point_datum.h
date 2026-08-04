#pragma once
/* hud_nav_point_datum — one HUD nav-point slot (DB layout, 12 bytes). The 16-bit word at +2 is
 * bit-packed: bits [7:0] reserved padding, [11:8] screen_type (on-screen render category), and
 * [15:12] type (nav-point kind; the signed 4-bit field reads 0xF as -1 == deactivated/invalid). */

typedef struct hud_nav_point_datum
{
    __int16          nav_index;         /* 0x0 */
    unsigned __int16 _reserved : 8;     /* 0x2 — reserved padding, bits [7:0] (DB anonymous bitfield) */
    __int16          screen_type : 4;   /* 0x2 — bits [11:8], on-screen render category */
    __int16          type : 4;          /* 0x2 — bits [15:12], nav-point type (0xF/-1 == deactivated) */
    float            z_offset;          /* 0x4 */
    int              reference_index;   /* 0x8 */
} hud_nav_point_datum;                  /* 12 bytes */

/* Composite per-player table (hud_nav_point_datum[4]) lives in its own header; included after the
 * base type is fully defined so consumers of this header keep transitively seeing both types
 * (pragma-once makes the child's reciprocal include of this file a no-op). */
#include "hud_nav_point_player_datum.h"
