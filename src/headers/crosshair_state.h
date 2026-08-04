#pragma once
/* crosshair_state — one 4-byte runtime slot per weapon-HUD crosshair overlay. The stored value is
 * state-index dependent, so the slot is a union: an integer reference_data (a game_time timestamp,
 * a countdown, a zoom sub-value, or the -1 "inactive" sentinel) or a float frame_index. DB spells
 * the slot as anonymous-union member ___u0 of type _EF6E8CBC4EA77CF585D8AFD4AB5DA894 ($-name kept
 * verbatim; $ in identifiers is an MSVC extension — argb_color.h convention). */

/* DB union _EF6E8CBC4EA77CF585D8AFD4AB5DA894 — the two value shapes overlaid. */
typedef union _EF6E8CBC4EA77CF585D8AFD4AB5DA894
{
    int   reference_data;  /* 0x00 */
    float frame_index;     /* 0x00 */
} _EF6E8CBC4EA77CF585D8AFD4AB5DA894;

typedef struct crosshair_state
{
    _EF6E8CBC4EA77CF585D8AFD4AB5DA894 ___u0;  /* 0x00 */
} crosshair_state;             /* 0x04 = 4 bytes */
