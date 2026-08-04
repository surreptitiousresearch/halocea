#ifndef DRAW_TEXT_FLAGS_H
#define DRAW_TEXT_FLAGS_H

/* draw_text_flags - bit indices for the draw_text.flags word.
 * DB-verified: types_enum_values _878CBA137C11E774740A67D3C8F943B4 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum draw_text_flags {
	_draw_text_wrap_horizontally_bit = 0,
	_draw_text_wrap_vertically_bit = 1,
	_draw_text_center_vertically_bit = 2,
	_draw_text_bottom_justify_bit = 3,
	NUMBER_OF_TEXT_FLAGS = 4,
};

#endif /* DRAW_TEXT_FLAGS_H */

// draw_text_flags: DB-verified via types_enum_values _878CBA137C11E774740A67D3C8F943B4
