#ifndef BITMAP_FLAGS_H
#define BITMAP_FLAGS_H

/* bitmap_flags — bitmap flags.
 * DB-verified: types_enum_values _191ED75248CF1BA3EB88BBAF94D11E12 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum bitmap_flags {
	_bitmap_has_power_of_two_dimensions_bit = 0,
	_bitmap_compressed_bit = 1,
	_bitmap_palettized_bit = 2,
	_bitmap_swizzled_bit = 3,
	_bitmap_linear_bit = 4,
	_bitmap_format_v16u16_bit = 5,
	NUMBER_OF_ON_DISK_BITMAP_FLAGS = 6,
	_bitmap_free_on_delete_bit = 6,
	_bitmap_cached_bit = 7,
	_bitmap_data_file_cache_bit = 8,
	NUMBER_OF_BITMAP_FLAGS = 9,
};

#endif /* BITMAP_FLAGS_H */

// bitmap_flags: DB-verified via types_enum_values _191ED75248CF1BA3EB88BBAF94D11E12
