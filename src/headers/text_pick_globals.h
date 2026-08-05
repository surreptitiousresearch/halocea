#pragma once
// DB-verified via types_members _563FA2A25DF601B361DB7AFEB0737E24 (anonymous PDB type)
/* _text_pick_globals — scratch state for draw_string_pick's hit-testing pass over text_pick_draw_character.
 * DB layout, anonymous struct (10 bytes). */

#include <stdint.h>
#include "point2d.h"

typedef struct _text_pick_globals
{
    point2d          pick_point;             /* 0x0 */
    int16_t          best_pick_string_index; /* 0x4 */
    int16_t          best_pick_distance;     /* 0x6 */
    int16_t          last_string_index;      /* 0x8 */
} _text_pick_globals; /* 10 bytes */
