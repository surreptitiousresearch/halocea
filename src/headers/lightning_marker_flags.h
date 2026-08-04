#ifndef LIGHTNING_MARKER_FLAGS_H
#define LIGHTNING_MARKER_FLAGS_H

/* lightning_marker_flags — lightning_marker flags.
 * DB-verified: types_enum_values _19B16D0E6AAA2C406AC7E859E0135C42 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum lightning_marker_flags {
	_lightning_marker_not_connected_to_next_marker_bit = 0,
	NUMBER_OF_LIGHTNING_MARKER_FLAGS = 1,
};

#endif /* LIGHTNING_MARKER_FLAGS_H */

// lightning_marker_flags: DB-verified via types_enum_values _19B16D0E6AAA2C406AC7E859E0135C42
