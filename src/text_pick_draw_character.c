/* text_pick_draw_character @0x837699A8 — the "hit-testing" substitute for the real character-draw
 * callback, invoked by the same string-layout pass while in text-picking mode (see
 * _text_pick_globals). For each laid-out character's bounding box [x0,x0+dx) x [y0,y0+dy), computes the
 * Chebyshev distance from text_pick_globals.pick_point to the box and, if it's the closest seen so far,
 * records which string index the pick point landed in — using the left/right half of the box to decide
 * whether the pick point should resolve to this character or the previous one (a standard "click left
 * half of a glyph selects before it, right half selects after it" text-cursor convention). Ignores the
 * font/color/x/y draw parameters entirely, since nothing is actually drawn in picking mode.
 *
 * DEVIATION: the decompiler fabricates 20 extra trailing parameters (a11..a30) beyond the DB's real
 * 10-parameter prototype; only x0/y0 (used directly) and dx/dy (renamed here from the fabricated a28/a30,
 * matching their positional role as the tail of the real signature and their use as box-extent additions
 * to x0/y0) are load-bearing. The XOR/unsigned-compare half-selection expression is a standard
 * signed-comparison-via-unsigned-trick idiom, simplified here to a plain "pick_point.x - x0 < dx/2" test. */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/text_pick_globals.h"
#include "headers/blam_data_globals.h"


extern __int16 abs16(__int16 value);

void text_pick_draw_character(parse_string_state *parse_state, font_header *font_header, font_character *font_character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy)
{
    __int16 x1 = x0 + dx;
    __int16 y1 = y0 + dy;

    __int16 dist_x0 = abs16(x0 - text_pick_globals.pick_point.n[0]);
    __int16 dist_x1 = abs16(x1 - text_pick_globals.pick_point.n[0]);
    __int16 dist_y0 = abs16(y0 - text_pick_globals.pick_point.n[1]);
    __int16 dist_y1 = abs16((__int16)(y1 - text_pick_globals.pick_point.n[1]));

    __int16 max_x_distance = (dist_x0 > dist_x1) ? dist_x0 : dist_x1;
    __int16 distance = (max_x_distance <= dist_y0) ? dist_y0 : max_x_distance;
    distance = (distance <= dist_y1) ? dist_y1 : distance;

    if ( distance < text_pick_globals.best_pick_distance )
    {
        text_pick_globals.best_pick_distance = distance;

        if ( (text_pick_globals.pick_point.n[0] - x0) < (dx >> 1) )
        {
            text_pick_globals.best_pick_string_index = text_pick_globals.last_string_index;
            text_pick_globals.last_string_index = parse_state->string_index;
            return;
        }

        text_pick_globals.best_pick_string_index = parse_state->string_index;
    }

    text_pick_globals.last_string_index = parse_state->string_index;
}
