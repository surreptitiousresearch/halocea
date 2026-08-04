#pragma once
/* hud_number_definition — the "hud digits" interface tag: the shared bitmap and per-glyph metrics used to
 * draw numeric HUD readouts (100 bytes, DB types_members-confirmed). */

#include "tag_reference.h"

typedef struct hud_number_definition
{
    tag_reference number_bitmap;         /* 0x00 — digits bitmap group */
    char          character_width;       /* 0x10 */
    char          screen_width;          /* 0x11 */
    char          x_offset;              /* 0x12 */
    char          y_offset;              /* 0x13 */
    char          decimal_point_width;   /* 0x14 */
    char          colon_width;           /* 0x15 */
    __int16       pad;                    /* 0x16 */
    int           unused[19];             /* 0x18 */
} hud_number_definition;                  /* 0x64 = 100 bytes */
