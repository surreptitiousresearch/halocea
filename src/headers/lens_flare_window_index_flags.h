#ifndef LENS_FLARE_WINDOW_INDEX_FLAGS_H
#define LENS_FLARE_WINDOW_INDEX_FLAGS_H

/* lens_flare_window_index_flags - bit indices for the lens_flare_window_index.flags word.
 * DB-verified: types_enum_values _BFD1FBC066805A27BA0E2D6EF2E2F46E carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum lens_flare_window_index_flags {
	_lens_flare_window_index_first_person_bit = 7,
	_lens_flare_window_index_mask = -129,
};

#endif /* LENS_FLARE_WINDOW_INDEX_FLAGS_H */

// lens_flare_window_index_flags: DB-verified via types_enum_values _BFD1FBC066805A27BA0E2D6EF2E2F46E
