/* draw_string_pick @0x8376B4B8 — hit-test a point against a drawn string: seed text_pick_globals with the
 * probe point and re-run draw_string with the text_pick_draw_character callback, which records the closest
 * glyph's string index as it walks. */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/text_pick_globals.h"
#include "headers/blam_data_globals.h"


#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
extern void text_pick_draw_character(parse_string_state *parse_state, font_header *font_header, font_character *font_character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy);
extern void draw_string(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust, const char *string);

int16_t draw_string_pick(const rectangle2d *bounds, const char *string, const point2d *point)
{
    text_pick_globals.pick_point = *point;
    text_pick_globals.best_pick_distance = 0x7FFF;
    text_pick_globals.best_pick_string_index = 0;
    text_pick_globals.last_string_index = 0;

    draw_string(text_pick_draw_character, bounds, 0, 0, 0, string);
    return (uint16_t)text_pick_globals.best_pick_string_index;
}
