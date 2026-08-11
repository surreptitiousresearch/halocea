/* text_bounds_draw_character @0x83769910 — the per-character callback used by draw_unicode_string when
 * measuring (rather than drawing) a string. It does not render; instead it grows the accumulated
 * bounding rectangle in text_bounds_globals to include this glyph's box (origin x0,y0; extent dx,dy) and
 * records the glyph's font header as the last one seen.
 *
 * DEVIATION: the decompiler invented a long tail of phantom args (a11..a30); the DB prototype has ten.
 * The two it actually reads off the stack — a28 and a30 — are the glyph extents dx and dy. */

#include <stdint.h>
#include "headers/text_bounds_globals.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/blam_data_globals.h"

struct parse_string_state;


void text_bounds_draw_character(struct parse_string_state *parse_state, font_header *font_header, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy)
{
    if (x0 < text_bounds_globals.bounds.n[1])
        text_bounds_globals.bounds.n[1] = x0;
    if (y0 < text_bounds_globals.bounds.n[0])
        text_bounds_globals.bounds.n[0] = y0;
    if ((int16_t)(dx + x0) > text_bounds_globals.bounds.n[3])
        text_bounds_globals.bounds.n[3] = dx + x0;
    text_bounds_globals.last_font_header = font_header;
    if ((int16_t)(dy + y0) > text_bounds_globals.bounds.n[2])
        text_bounds_globals.bounds.n[2] = dy + y0;
}
